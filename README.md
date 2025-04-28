# Pack It UP! - Never run out of beer again.
# THIS WILL BE A LIBADWAITA PORT FOR GNOME
# WORK IN PROGRESS
[![License](https://img.shields.io/badge/License-GPLv3-blue)](./LICENSE)
[![English](https://img.shields.io/badge/README-en--US-blue)](./README.md)
[![Português](https://img.shields.io/badge/README-pt--BR-green)](./README.pt-BR.md)
### ***Packages names might be wrong, still testing*** 
#### Tested on:
- [x] Ubuntu
- [ ] Fedora Workstation
- [ ] OpenSUSE Tumbleweed
- [x] Arch Linux
- [ ] Alpine Linux
- [ ] Void Linux
## KDE Plasma/Kubuntu
Although the app work in KDE environment, theming it might be tricky and it might contain minor glitches. It doesn't contain LibAdwaita though, making KDE lovers life easier.
## Install
At the moment the only way to install is by cloning the repository:

1. Install [Build prerequisites](#build-prerequisites)
2. `git clone https://github.com/edu-bm7/packitup`
3. `cd packitup`
4. `meson setup builddir`
    - At the moment you could install it with a custom location, but localization packages won't work(Translation packages). 
    - If you wish to do it anyway, run `meson setup --prefix=YOUR_CUSTOM_INSTALL_PATH builddir`
5. `ninja -C builddir`
6. `sudo ninja -C builddir install`

## Uninstall
To uninstall our program, if you have compiled it with `ninja` you can use:
```
sudo ninja -C builddir uninstall
```
Where `builddir` is the build directory you specified with `meson setup`


## Build prerequisites

- GCC 8+ or Clang 5+ (C++17 support)

### Ubuntu
```sh
sudo apt update
sudo apt install -y \
  fonts-noto \
  libgtk-4-1 libgtk-4-dev \
  libgtkmm-4.0-0 libgtkmm-4.0-dev \
  libadwaita-1-0 libadwaita-1-dev \
  build-essential meson ninja-build curl \
  gettext libxml2-utils \
  pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Fedora/RHEL
```
sudo dnf install -y \
  google-noto-fonts-common \
  gtk4 gtk4-devel gtkmm4.0 gtkmm4.0-devel \
  @c-development @development-tools \
  libadwaita libadwaita-dev
  libxml2 gettext \
  meson ninja-build curl pkgconf-pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### OpenSUSE Tumbleweed
```
sudo zypper refresh
sudo zypper install -y \
  google-noto-fonts \
  gtk4 gtkmm4 gtkmm4 \
  meson ninja curl gcc-c++ pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Arch Linux
```
sudo pacman -Syu --needed \
  noto-fonts \
  base-devel meson ninja curl pkgconf python \
  gtk4 gtkmm-4.0 \
  libxml gettext \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Alpine Linux
```
apk add \
    font-noto-all
    build-base pkgconf meson ninja-build python3 curl \
    gtk4.0 gtk4.0-dev \
    xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Void Linux
```
sudo xbps-install -Sy \
  noto-fonts-ttf \
  gtk4 gtk4-devel gtkmm4 gtkmm4-devel \
  base-devel meson ninja curl gcc pkgconf python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Window Manager in X11
- You'll need a compositor to handle shadow, transparency, stacking and other effects.
E.g. `picom`

### Fonts
- To run the app in a language that is not english(right now only pt-BR is available)
you'll probally will need a font that supports it. E.g `noto-fonts`
