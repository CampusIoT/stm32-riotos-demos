# Configuration du projet avec STM32CubeIDE


0) Télécharger et installer STM32CubeIDE https://www.st.com/en/development-tools/stm32cubeide.html
1) Importer le projet via File→New→Makefile Project with Existing Code (selectionner la toolchain MCU ARM GCC)
2) Ajouter  la variable d’environnement RIOT_BASE (chez moi /Users/donsez/github/RIOT-OS/RIOT) via Project→Properties Then select C/C++ Build→Environnement
3) créer la configuration .xml avec make BOARD=im880b eclipsesym
4)importer la configuration via Project→Properties Then select C/C++ General→Paths and Symbols

Il est possible d’ouvrir depuis Eclipse le terminal série via l’UART de la carte. Cependant, il faut prévoir de mettre des carnage returns dans les traces (\n\r au lieu de \n) car ceci ne semble pas configurable dans le terminal.

# MacOS
RIOT requière une version 4.x de Make.

GNU Make 3.81 is not supported by RIOT since release 2020.01. Please upgrade your system to use GNU Make 4.% or later..


```bash
make --version
brew install gmake
gmake --version

```
