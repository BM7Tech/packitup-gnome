# Pack It UP!
# ** WORK IN PROGRESS **
## ** Packages names might be wrong, still testing ** 
### Build prerequisites

- GCC 8+ or Clang 5+ (C++17 support)

### Window Manager in X11
- You'll need a compositor to handle shadow, transparency, stacking and other effects.
E.g. `picom`

### Fonts
- To run the app in a language that is not english(right now only pt-BR is available)
you'll probally will need a font that supports it. E.g `noto-fonts`

### Ubuntu/Debian
```sh
sudo apt update
sudo apt install -y \
  fonts-noto \
  libgtk-4-0 libgtk-4-dev \
  libgtkmm-4.0-1v5 libgtkmm-4.0-dev \
  build-essential meson ninja-build curl \
  pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Fedora/RHEL
```
sudo dnf install -y \
  google-noto-fonts-common \
  gtk4 gtk4-devel gtkmm4 gtkmm4-devel \
  meson ninja-build curl gcc-c++ pkgconf-pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### OpenSUSE
```
sudo zypper refresh
sudo zypper install -y \

  gtk4 gtk4-devel gtkmm4 gtkmm4-devel \
  meson ninja curl gcc-c++ pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Arch Linux
```
sudo pacman -Syu --needed \
  noto-fonts \
  base-devel meson ninja curl pkgconf python \
  gtk4 gtkmm-4.0 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Alpine Linux
```
apk add \
    build-base pkgconf meson ninja-build python3 curl \
    gtk4.0 gtk4.0-dev \
    xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Void Linux
```
sudo xbps-install -Sy \
  gtk4 gtk4-devel gtkmm4 gtkmm4-devel \
  base-devel meson ninja curl gcc pkgconf python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```




