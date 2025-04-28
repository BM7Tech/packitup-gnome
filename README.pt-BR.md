
# Pack It UP! - Nunca mais fique sem cerveja.
# ESSA FORK SERA UM PROJETO FEITO COM COMPATIBILIDADE PARA LIBADWAITA GNOME
# Em Desenvolvimento
[![License](https://img.shields.io/badge/License-GPLv3-blue)](./LICENSE)
[![English](https://img.shields.io/badge/README-en--US-blue)](./README.md)
[![Português](https://img.shields.io/badge/README-pt--BR-green)](./README.pt-BR.md)
### ***Nomes dos pacotes podem estar errado, ainda estou testando*** 
#### Testado em:
- [x] Ubuntu/Kubuntu
- [ ] Fedora Workstation
- [ ] OpenSUSE Tumbleweed
- [x] Arch Linux
- [ ] Alpine Linux
- [ ] Void Linux
## KDE Plasma/Kubuntu
Apesar do aplicativo funcionar no ambiente Desktop KDE, mudar a aparência da aplicação para combinar com o seu Desktop pode ser um tanto quanto complicado e conter alguns pequenos bugs. Entretando o aplicativo não contém a livraria LibAdwaita, facilitando a vida daqueles que amam KDE.
## Instalar
No momento a única maneira de instalar o nosso programa é clonando o repositório:

1. Instale as [Dependências de compilação](#build-prerequisites)
2. `git clone https://github.com/edu-bm7/packitup`
3. `cd packitup`
4. `meson setup builddir`
    - Se quiser você pode escolher um diretório customizado para instalação, porém, os pacotes de tradução não irão funcionar. 
    - Se quiser fazê-lo mesmo assim, `meson setup --prefix=SEU_DIRETORIO builddir`
5. `ninja -C builddir`
6. `sudo ninja -C builddir install`

## Desinstalar
Para desinstalar nosso programa, caso tenha compilado com `ninja` você pode usar o comando:
```
sudo ninja -C builddir uninstall
```
Onde `builddir` é o diretório em que você especificou em `meson setup`


## Requisítos para compilação

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
  libadwaita libadwaita-devel \
  @c-development @development-tools \
  libxml2 gettext \
  meson ninja-build curl pkgconf-pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### OpenSUSE Tumbleweed
```
sudo zypper refresh
sudo zypper install -y \
  google-noto-fonts \
  gtk4 gtkmm4 \
  meson ninja curl gcc-c++ pkg-config python3 \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Arch Linux
```
sudo pacman -Syu --needed \
  noto-fonts \
  base-devel meson ninja curl pkgconf python \
  gtk4 gtkmm-4.0 \
  libxml2 gettext \
  xdg-desktop-portal xdg-desktop-portal-gtk xdg-utils
```

### Alpine Linux
```
apk add \
    font-noto-all
    build-base pkgconf meson ninja-build python3 curl \
    gtk4.0 gtk4.0-dev gtkmm4 gtkmm4-dev \
    libxml2 gettext \
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

### Gerenciadores de Janela no X11
- Você irá precisar de um compositor para gerenciar sombras, transparência, stacking e outros efeitos.
Ex: `picom`

### Fontes
- Para executar nosso aplicativo em uma lingaguem que não seja o inglês (no momento apenas português brasileiro é suportado) você provalvelmente precisará de um pacote de fontes que suportam os caracteres dessa lingua. Ex: `noto-fonts`

