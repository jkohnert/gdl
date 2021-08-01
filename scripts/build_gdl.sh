#!/usr/bin/env bash
#
# script to download some files needed for mingw32, and build GDL.
#
# JP:
#  Since old packages are not available from msys2 repo anymore, let's try the latest versions here.
#  If they don't work (especially readline), we need to store the old ones in another location.
#  Below is the list of libraries had to be 'reversioned' in the previous version of this script:
#   - readline, zlib, libpng, libpcre
#
# 2021-03-20: Script has been updated to support Linux and macOS
LANG=C #script works only in english
ME="build_gdl.sh"
Configuration=${Configuration:-"Release"}
DEPS=${DEPS:-"standard"}
real_path() {
    [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}
export GDL_DIR=$(real_path "$(dirname $0)/..")
export ROOT_DIR=${ROOT_DIR:-"${GDL_DIR}"}
INSTALL_PREFIX=${INSTALL_PREFIX:-"${ROOT_DIR}/install"}
PYTHONVERSION=${PYTHONVERSION:-"3"}
GDLDE_VERSION=${GDLDE_VERSION:-"v1.0.0"} # needed by 'pack' (at the moment Windows only)
NTHREADS=${NTHREADS:-$(getconf _NPROCESSORS_ONLN)} # find nthreads for make -j
BUILD_OS=$(uname)
DRY_RUN=false
if [[ ${BUILD_OS} == *"MSYS"* ]]; then
    BUILD_OS="Windows"
elif [[ ${BUILD_OS} == *"MINGW"* ]]; then
    BUILD_OS="Windows"
elif [[ ${BUILD_OS} == "Darwin" ]]; then
    BUILD_OS="macOS"
fi

function log {  # log is needded just below!
    echo "[${ME}] $@"
}

if [ ${BUILD_OS} == "Windows" ]; then
    BSDXDR_URL="https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/bsd-xdr/bsd-xdr-1.0.0.tar.gz"
    MSYS2_PACKAGES=(
        readline zlib libpng gsl wxWidgets plplot libgd libtiff libgeotiff netcdf hdf4 hdf5 fftw proj msmpi udunits
        eigen3 eccodes glpk shapelib expat openssl
    )
    MSYS2_PACKAGES_REBUILD=(
        graphicsmagick
    )
elif [ ${BUILD_OS} == "Linux" ]; then
    # JP: Note the seperator ',' between the package name candidates below. The leftmost one has the highest priority.
    # Debian, Ubuntu, Linux Mint, Elementary OS, etc.
    APT_PACKAGES=(
        libncurses-dev libreadline-dev,libreadline-gplv2-dev zlib1g-dev libpng-dev libgsl-dev,libgsl0-dev
        libwxgtk3.0-gtk3-dev,libwxgtk3.0-dev,libwxgtk2.8-dev libplplot-dev libgraphicsmagick++1-dev,libgraphicsmagick++-dev libtiff-dev
        libgeotiff-dev libnetcdf-dev libhdf4-alt-dev libhdf5-dev libfftw3-dev libproj-dev libopenmpi-dev libpython3-dev,libpython-dev python3-dev,python-dev
        python3-numpy,python-numpy libudunits2-dev libeigen3-dev libeccodes-dev,libgrib-api-dev libglpk-dev libshp-dev,shapelib libexpat1-dev
    )
    # Redhat, Fedora, Scientific Linux, CentOS, openSuSE, SLES, etc.
    RPM_PACKAGES=(
        libtirpc-devel ncurses-devel readline-devel zlib-devel libpng-devel,libpng16-devel gsl-devel
	wxGTK3-devel,wxGTK-devel,wxWidgets-3_2-devel,wxWidgets-3_0-devel
        plplot-wxGTK-devel,plplotwxwidgets-devel plplot-driver-xwin plplot-driver-wxwidgets plplot-driver-svg plplot-driver-ps 
        GraphicsMagick-c++-devel,libGraphicsMagick++-devel libtiff-devel libgeotiff-devel,libgeotiff5-devel,geotiff-devel 
        netcdf-devel hdf-devel hdf5-devel fftw-devel,fftw3-devel proj-devel openmpi-devel,openmpi4-devel,openmpi3-devel
	python39-devel,python38-devel,python3-devel,python-devel
        python39-numpy-devel,python38-numpy-devel,python3-numpy-devel,python-numpy-devel,python39-numpy,python38-numpy,python3-numpy,python-numpy
	udunits2-devel eigen3-devel eccodes-devel,grib_api-devel glpk-devel libshp-devel,shapelib-devel
        expat-devel,libexpat-devel
    ) # JP 2021 Mar 21: SuSE lacks eccodes
elif [ ${BUILD_OS} == "macOS" ]; then
    BREW_PACKAGES=(
        llvm libomp ncurses readline zlib libpng gsl wxmac graphicsmagick libtiff libgeotiff netcdf hdf5 fftw proj open-mpi python numpy udunits eigen
        eccodes glpk shapelib expat gcc@10
    ) # JP 2021 Mar 21: HDF4 isn't available - not so critical I guess
      # JP 2021 May 25: Added GCC 10 which includes libgfortran, which the numpy tap relies on.
else
    log "Fatal error! Unknown OS: ${BUILD_OS}. This script only supports one of: Windows, Linux, macOS."
    exit 1
fi

function download_file {
    package_url=$1
    package_file=$(basename $package_url)
    log "Downloading ${package_file}..."
    if [ -f $package_file ]; then
       log "File exists! Skipping..."
    else
        curl -LOs --retry 5 $package_url
        if [ ! $? -eq 0 ]; then
            log "Failed to download ${package_file}!"
            exit 1
        fi
    fi
}

function decompress_file {
    log "Decompressing ${package_file}..."
    if [ ${package_file: -4} == ".zip" ]; then
        unzip -q $@ $package_file
    else
        tar xf $@ $package_file
    fi
    if [ ! $? -eq 0 ]; then
        log "Failed to decompress ${package_file}!"
        exit 1
    fi
}

function build_msys2_package {
    log "Building package $1..."
    pushd ${ROOT_DIR}
    if [[ ! -d "MINGW-packages" ]]; then
        git clone \
            --depth 1  \
            --filter=blob:none  \
            --sparse \
            https://github.com/msys2/MINGW-packages \
            ;
    fi
    cd MINGW-packages
    git sparse-checkout init --cone
    rm -fr mingw-w64-$1
    git sparse-checkout set mingw-w64-$1
    git checkout mingw-w64-$1
    cd mingw-w64-$1
    patch="${GDL_DIR}/scripts/deps/windows/mingw-patches/$1.patch"
    if [[ -f $patch ]]; then
        patch -p0 -i $patch
    fi
    if [[ ${arch} == "x86_64" ]]; then
        export MINGW_ARCH=mingw64
    else
        export MINGW_ARCH=mingw32
    fi
    makepkg-mingw --noconfirm --syncdeps --install
    popd
}

function find_architecture {
    if [ ${Platform:-`uname -m`} == "x86_64" ]; then
        export mname="mingw64"
        export MSYSTEM="MINGW64"
        export arch="x86_64"
    else
        export mname="mingw32"
        export MSYSTEM="MINGW32"
        export arch="i686"
    fi
    export PATH=/$mname/bin:$PATH
    log "Architecture: ${arch}"
}

function install_zstd {
    mkdir -p /c/projects/zstd
    cd /c/projects/zstd
    zstd_version="v1.4.8"
    if [ $arch == "i686" ]; then
        zstd_zip=zstd-${zstd_version}-win32.zip
    else
        zstd_zip=zstd-${zstd_version}-win64.zip
    fi
    curl -LOs https://github.com/facebook/zstd/releases/download/${zstd_version}/${zstd_zip}
    7z x ${zstd_zip}
    cp zstd.exe /usr/bin/
}

function query_package {
    if [ ${PKGMGR} == "apt" ]; then
        test_package=( `apt-cache policy $1 | grep Installed:` )
        if [ ! -n "$test_package" ]; then
            query_result=0 # invalid package name
        elif [[ ${test_package[1]} == "(none)" ]]; then
            query_result=1 # the package exists, but not installed
        else
            query_result=2 # the package is already installed
        fi
    elif [ ${PKGMGR} == "yum" ]; then
        test_package=`yum info -C --available $1 2>&1 | grep Error`
        if [ ! -n "$test_package" ]; then
            # the package exists
            test_package2=`yum info -C --installed $1 2>&1 | grep Error`
            if [ ! -n "$test_package2" ]; then
                query_result=2 # the package is already installed
            else
                query_result=1 # the package exists, but not installed
            fi
        else
            query_result=0 # invalid package name
        fi
    elif [ ${PKGMGR} == "zypper" ]; then
        test_package=`zypper info $1`
        if [[ ${test_package} == *"not found"* ]]; then
            query_result=0 # invalid package name
        else
            if [[ ${test_package} == *"not installed"* ]]; then
                query_result=1 # the package exists, but not installed
            else
                query_result=2 # the package is already installed
            fi
        fi
    fi
}

function find_pkgmgr {
    PKGINSTALLARG="install"
    if command -v apt >/dev/null 2>&1; then
        PACKAGES=( ${APT_PACKAGES[@]} )
        PKGMGR="apt"
    else
        PACKAGES=( ${RPM_PACKAGES[@]} )
        if command -v yum >/dev/null 2>&1; then
            PKGMGR="yum"
        elif command -v zypper >/dev/null 2>&1; then
            PKGMGR="zypper"
        else
            log "Fatal error! Cannot determine package manager on your system."
            exit 1
        fi
    fi
    log "Found package manager: $PKGMGR"
    if [ ${PKGMGR} == "apt" ]; then
        log "Updating apt package cache..."
        sudo apt update
    fi
}

function install_toolchain {
    log "Installing toolchain..."
    
    find_pkgmgr

    if [ ${BUILD_OS} == "Windows" ]; then
        pacman -S --noconfirm --needed mingw-w64-${arch}-toolchain mingw-w64-${arch}-cmake mingw-w64-${arch}-nsis unzip tar zstd make
    elif [ ${BUILD_OS} == "Linux" ]; then
        if [ ${PKGMGR} == "apt" ]; then
            INSTALL_PACKAGES="g++ cmake make"
        elif [ ${PKGMGR} == "yum" ]; then
            INSTALL_PACKAGES="gcc-c++ cmake make"
        elif [ ${PKGMGR} == "zypper" ]; then
            INSTALL_PACKAGES="gcc-c++ cmake make"
        else
            log "Fatal error! Cannot determine package manager on your system."
            exit 1
        fi
        eval "sudo ${PKGMGR} ${PKGINSTALLARG} -y ${INSTALL_PACKAGES}"
    fi
}

function prep_packages {
    if [ ${BUILD_OS} == "Windows" ]; then
        for pkgname in ${MSYS2_PACKAGES[@]}; do
            msys2_packages="${msys2_packages} mingw-w64-${arch}-${pkgname}"
        done
        
        log "Installing dependencies: ${msys2_packages}"
        if [ ${DRY_RUN} == "true"]; then
            log "Please run below command to install required packages to build GDL."
            echo "pacman --noconfirm --needed -S ${msys2_packages}"
        else
            eval "pacman --noconfirm --needed -S ${msys2_packages}"
        fi

        for package_name in ${MSYS2_PACKAGES_REBUILD[@]}; do
            build_msys2_package $package_name
        done

        log "Patching wx-config..."
        sed -e "s;-Wl,--subsystem,windows -mwindows;;" -i /${mname}/bin/wx-config

        download_file ${BSDXDR_URL}
        decompress_file

        log "Building bsd-xdr..."
        pushd bsd-xdr-1.0.0
        sed -e 's/-Wall/-Wall -Wno-pointer-to-int-cast #/' -e 's/$(XDR_LIBRARIES) $(TEST_PROGS)/$(XDR_LIBRARIES)/' -e 's/libxdr/libbsdxdr/' -i Makefile
        mv lib/libxdr.def.in lib/libbsdxdr.def.in
        make || exit 1
        cp -f mingw/*.dll /${mname}/bin/
        cp -f mingw/libbsdxdr.dll.a /${mname}/lib/
        cp -rf rpc /${mname}/include/
        popd
    elif [ ${BUILD_OS} == "Linux" ]; then
        # JP: This part is based on `aptget4gdl.sh` and `rpm4gdl.sh` by Alain C. and Ilia N.
        find_pkgmgr
        log "Checking package availabilities..."
        for pkgnamecandidates in ${PACKAGES[@]}; do
            for pkgname in $(echo $pkgnamecandidates | tr ',' ' '); do
                query_package $pkgname
                if [[ ${query_result} == "1" ]] ; then
                    INSTALL_PACKAGES="${INSTALL_PACKAGES} $pkgname"
                    break # install only one package
                elif [[ ${query_result} == "2" ]] ; then
                    break # the package is already installed
                fi
            done
        done
        if [[ -z ${INSTALL_PACKAGES} ]]; then
            log "All required packages are already installed on your system."
	else
            log "Installing packages:"
            log "${INSTALL_PACKAGES}"
            if [ ${DRY_RUN} == "true"]; then
                log "Please run below command to install required packages to build GDL."
                echo "sudo ${PKGMGR} ${PKGINSTALLARG} -y ${INSTALL_PACKAGES}"
            else
                eval "sudo ${PKGMGR} ${PKGINSTALLARG} -y ${INSTALL_PACKAGES}"
            fi
        fi
    elif [ ${BUILD_OS} == "macOS" ]; then
        if ! command -v brew >/dev/null 2>&1; then
            log "Fatal error! Homebrew not found."
            exit 1
        fi
        brew update-reset
        brew unlink python@2
        log "Installing packages: ${BREW_PACKAGES[@]}"
        if [ ${DRY_RUN} == "true"]; then
            log "Please run below command to install required packages to build GDL."
            echo "brew install ${BREW_PACKAGES[@]}"
        else
            eval "brew install ${BREW_PACKAGES[@]}"
        fi
        log "Installing plplot..."
        brew --cache plplot
        bash $GDL_DIR/scripts/deps/macos/brew_enable_wxwidgets
    fi
}

function prep_packages_dryrun {
    DRY_RUN=true
    prep_packages
}

function find_dlls {
  log "Analyzing DLL dependency of $1..."
  for dll in $(strings $1 | grep -i '[a-zA-Z0-9]\.dll$' | grep -v " " | grep -v $(basename $1)); do
    dll="/mingw64/bin/$dll"
    if [ -f "$dll" ] && [[ ! ${found_dlls[@]} =~ (^|[[:space:]])"$dll"($|[[:space:]]) ]]; then
      found_dlls+=("$dll")
      find_dlls "$dll";
        log "Found DLL dependency: $dll"
    fi
  done
}

function configure_gdl {
    log "Configuring GDL (${Configuration})..."

    if [[ ${DEPS} != "standard" && ${DEPS} != "headless" && ${DEPS} != "debug" ]]; then
        log "Fatal error! Unknown DEPS: ${DEPS}"
        exit 1
    fi

    mkdir -p ${ROOT_DIR}/build
    cd ${ROOT_DIR}/build

    if [ ${BUILD_OS} == "Windows" ]; then
        GENERATOR="MSYS Makefiles"
    else
        GENERATOR="Unix Makefiles"
    fi
    
    if [[ ${BUILD_OS} == "macOS" ]]; then
        export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/opt/llvm/lib
        CMAKE_ADDITIONAL_ARGS=( "-DREADLINEDIR=/usr/local/opt/readline"
                                "-DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++"
                                "-DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang" )
    fi
    
    WITH_MPI="ON"
    if [[ ${DEPS} == "standard" ]]; then
        WITH_MPI="OFF"
    fi

    WITH_WXWIDGETS="ON"
    if [[ ${DEPS} == "headless" ]]; then
        WITH_WXWIDGETS="OFF"
    fi

    if [[ ${BUILD_OS} == "macOS" ]]; then
        WITH_HDF4="OFF"
    else 
        zz=`grep -i opensuse /etc/*-release 2> /dev/null`
        if [[ -n $zz ]]; then
            # in case of openSUSE
            WITH_HDF4="OFF"
            WITH_GRIB="OFF"
        else
            # other distros
            WITH_HDF4="ON"
            WITH_GRIB="ON"
        fi
    fi

    #interactive graphics added as we do not want the compilation to fail on systems where plplot is not correctly installed. The intent was to
    #force distro packagers to include the plplot drivers in the dependency of the GDL package, not annoy the users of this script.
    cmake ${GDL_DIR} -G"${GENERATOR}" \
        -DCMAKE_BUILD_TYPE=${Configuration} \
        -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG" \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
        -DWXWIDGETS=${WITH_WXWIDGETS} -DGRAPHICSMAGICK=ON \
        -DNETCDF=ON -DHDF=${WITH_HDF4} -DHDF5=ON \
        -DMPI=${WITH_MPI} -DTIFF=ON -DGEOTIFF=ON \
        -DLIBPROJ=ON -DPYTHON=ON -DPYTHONVERSION=${PYTHONVERSION} -DFFTW=ON \
        -DUDUNITS2=ON -DGLPK=ON -DGRIB=${WITH_GRIB} \
        -DUSE_WINGDI_NOT_WINGCC=ON -DINTERACTIVE_GRAPHICS=OFF ${CMAKE_ADDITIONAL_ARGS[@]}
}

function build_gdl {
    if [ ! -d "${ROOT_DIR}/build" ]; then 
        log "Fatal error! Build directory not found."
        exit 1
    fi

    cd ${ROOT_DIR}/build

    log "Building GDL..."
    make -j${NTHREADS} || exit 1
}


function install_gdl {
    if [ ! -d "${ROOT_DIR}/build" ]; then 
        log "Fatal error! Build directory not found."
        exit 1
    fi

    cd ${ROOT_DIR}/build

    log "Installing GDL..."
    make install || exit 1

    cd ${ROOT_DIR}/install

    if [ ${BUILD_OS} == "Windows" ]; then
        log "Copying DLLs to install directory..."
        found_dlls=()
        find_dlls ${ROOT_DIR}/build/src/gdl.exe
        for f in ${found_dlls[@]}; do
            cp -f "$f" bin/
        done
        log "Copying plplot drivers to install directory..."
        mkdir -p share
        cp -rf /${mname}/share/plplot* share/
        log "Copying GraphicsMagick drivers to install directory..."
        mkdir -p lib
        cp -rf /${mname}/lib/GraphicsMagick* lib/ # copy GraphicsMagick dlls
        cp -f ${GDL_DIR}/COPYING .
    fi
}

function test_gdl {
    log "Testing GDL..."
    cd ${ROOT_DIR}/build
    if [ -f ${GDL_DIR}/CMakeModules/CodeCoverage.cmake ]; then
        make codecov
    else
        make test
    fi
}

function pack_gdl {
    log "Packaging GDL..."
    if [ ${BUILD_OS} == "Windows" ]; then
        if [ $arch == "x86_64" ]; then
            cd ${ROOT_DIR}
            download_file "https://github.com/gnudatalanguage/gdlde/releases/download/${GDLDE_VERSION}/gdlde.product-win32.win32.x86_64.zip"
            decompress_file -o -d gdlde
        fi

        mkdir -p ${ROOT_DIR}/package
        cd ${ROOT_DIR}/package

        export GDL_INSTALL_DIR=`cygpath -w ${ROOT_DIR}/install`
        export GDL_VERSION=`grep -oP 'set\(VERSION "\K.+(?="\))' ${GDL_DIR}/CMakeLists.txt`
        makensis -V3 ${GDL_DIR}/scripts/deps/windows/gdlsetup.nsi
    fi
}

function prep_deploy {
    if [ ${BUILD_OS} == "Windows" ]; then
        cd ${GDL_DIR}
        mv gdlsetup.exe gdlsetup-${BUILD_OS}-${arch}-${DEPS}.exe
    fi
    cd ${ROOT_DIR}/install
    zip -qr ${GDL_DIR}/gdl-${BUILD_OS}-${arch}-${DEPS}.zip *
    cd ${GDL_DIR}
}

AVAILABLE_OPTIONS="prep prep_dryrun configure build install check pack prep_deploy"
AVAILABLE_OPTIONS_prep=prep_packages
AVAILABLE_OPTIONS_prep_dryrun=prep_packages_dryrun
AVAILABLE_OPTIONS_configure=configure_gdl
AVAILABLE_OPTIONS_build=build_gdl
AVAILABLE_OPTIONS_install=install_gdl
AVAILABLE_OPTIONS_check=test_gdl
AVAILABLE_OPTIONS_pack=pack_gdl
AVAILABLE_OPTIONS_prep_deploy=prep_deploy

if [ "$#" -ne 1 ]; then
    echo "Usage: $ME `echo ${AVAILABLE_OPTIONS} | tr ' ' '|'`"
else
    for optkey in ${AVAILABLE_OPTIONS}; do
        if [ $optkey == "$1" ]; then
            if [ ${BUILD_OS} == "Windows" ]; then
                find_architecture
            else
                export arch="x86_64"
            fi
            cmd=AVAILABLE_OPTIONS_$optkey
            eval ${!cmd}
            break
        fi
    done
fi