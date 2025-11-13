# Performe a full erase
`STM32_Programmer_CLI -c port=SWD  mode=UR -e all  -rst`

# Upload programme
`STM32_Programmer_CLI -c port=SWD  reset=HWrst -rst -w BluePillPlus-Keyboard-BIOS.elf 0x8000000` : IDK why 0x8000000, probably the flash location.

# Usefull options : 
`-vb <int>` : Define verbose level log

`-c port=<connection>` : Connection type, can be SWD, usb, uart   
* `port=COMX`   
* `port=/dev/ttySO`   
* `port=USBX`  

`-rst` : Performe a Nreset  
`reset=<ResetType>` : Specify the reset type  
* `reset=HWrst` : Hardware Reset
* `reset=SWrst` : Software Reset
* `reset=Crst` : ? 

`-w` : Write file ? 

[Full documentation for CLI](PDF/um2237-stm32cubeprogrammer-software-description-stmicroelectronics.pdf)

# TODO : 
