# nvidia-probe

## What does it do?

This little utility sends a "Probe Hardware Now" command to every NVidia GPU on your system through X11.

## Why would I need this?

Because hotplugging displays doesn't work on your system but you can manually detect new displays by clicking on the "Detect Displays" button in the `nvidia-settings` utility.

## What command line arguments does it support?

By default `nvidia-probe` will send a probe command once and exit, however by using the `--loop` option you can get it to send that command periodically.

## Can it also do...?

No.

## What is the recommended usage?

Copy `nvidia-probe` utility (in the `bin` folder) somewhere on your $PATH (i.e.: in the `~/bin` directory) and copy the `nvidia-probe.desktop` file into the directory 
`~/.config/autostart`. Starting from the next time you log in you should be able to "hotplug" new displays from and to your device.

# Credits

99,9% of the code used in this utility were taken from NVidias `nivida-settings` utility. The only thing I did was too write the main.c file...
