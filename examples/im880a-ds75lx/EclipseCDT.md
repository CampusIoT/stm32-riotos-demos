# Configuration du projet avec Eclipse CDT

Lire https://github.com/RIOT-OS/RIOT/wiki/Using-the-Eclipse-IDE-for-C-and-CPP-Developers%2C-Howto

Importer le projet avec 





Properties > C/C++ Build > Environment
Ajouter à PATH /bin:/usr/bin:/usr/sbin:/sbin:/usr/local/bin
Ajouter à RIOT_BASE /Users/donsez/github/RIOT-OS/RIOT

Properties > C/C++ Build
Build command : gmake

Properties > C/C++ Build > Tool Chain Editor
Select Cross GCC


Properties > C/C++ General > Paths and Symbols
Import Setting
Ajouter eclipsesym.xml généré au moyen de gmake BOARD=im880b eclipsesym


Ajouter /usr/local/Cellar/arm-none-eabi-gcc/8-2018-q4-major/gcc/arm-none-eabi/include aux includes


Pour le Debug
/usr/local/Cellar/arm-none-eabi-gcc/8-2018-q4-major/gcc/bin/arm-none-eabi-gdb


Lancer ST-LINK_gdbserver avec la commande 
/Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.macos64_1.3.0.202002181050/tools/bin/ST-LINK_gdbserver -d -v -cp /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.macos64_1.3.0.202002181050/tools/bin/



# MacOS
RIOT requière une version 4.x de Make.

GNU Make 3.81 is not supported by RIOT since release 2020.01. Please upgrade your system to use GNU Make 4.% or later..


```bash
make --version
brew install gmake
gmake --version

```
