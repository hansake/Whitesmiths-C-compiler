This is a virtual machine for CO-IDRIS running in VirtualBox.

The exported VM filename is: co-idris_ms-dos.ova

When VirtualBox is installed, open co-idris_ms-dos.ova to import the VM.
Then start the VM, MS-DOS 6.22 will boot.

Start Idris with the batch file IDR.BAT

 C:\>idr

return to DOS with the command quit

 \# quit
 
 To copy files between Linux and CO-IDRIS (see: filecopy.txt)

Updates:
* Updated date command to handle year > 1999
* Added simple make command. Note that the Makefile has a different syntax. The command make is a script that uses pp for pre-processing and makeprog for dependencies.
* The file /adm/init modified to start in multiuser mode, login as "root" or "pat"
* Login enabled at com1 and com2, connection to physical or virtual ports in VirtualBox (see: terminals.txt)
* The emacs like editor mince added.
* Cross compiler for CP/M-80 added, the C compiler command is ccpm80.
