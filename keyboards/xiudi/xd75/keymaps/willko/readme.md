# willko keymap for Xiudi xd75

All about color coded layers and the planck style layout with extra macros.

Macros are defined with mac osx in mind, but can be easily modified for windows if desired.




## Layout

![willko home layout](https://i.imgur.com/khxkgv7.png)




## Compile

```
make xiudi/xd75:willko
```

or

```
qmk compile -kb xiudi/xd75 -km willko
```

Note: The `xiudi_xd75_willko.hex.hex` output file should be in the `./.build` folder. 




## Flash

```
make xiudi/xd75:willko:flash
```

or 

```
qmk flash -kb xiudi/xd75 -km willko
```




## Macros and Special Keys

### Undo

On Mac OSX: `Cmd` + `z`,
Otherwise: `Ctrl` + `z`

### Apps (switcher)

On Mac OSX: `Cmd` + `tab`,
Otherwise: `Ctrl` + `tab`

It will wait for a breif period before releasing the os specific modifier.

### Redo

On Mac OSX: `Cmd` + `SHFT` + `Z`,
Otherwise: `Ctrl` + `SHFT` + `Z`

### F22

Left as the F22 key for software/os level macro mapping.

### Mode (tap dance functions)

Change the mode of the keyboard { game/work, osx/windows }

Mode Tap Dances:
- 1 -> Open the last closed tab
    - -> On Mac OSX: `Cmd` + `SHFT` + `T`
    - -> Otherwise: `Ctrl` + `SHFT` + `T`
- 2 -> Open the command palette (text editor macro)
    - -> On Mac OSX: `Cmd` + `SHFT` + `P`
    - -> Otherwise: `Ctrl` + `SHFT` + `P`
- 3 -> Toggle OSX / Windows mode
    - -> Toggles OS modifier to/from: `Cmd`/`Ctrl`
- 5 -> Toggle Game / Work mode
    - -> Toggles base layer to either 'game' or 'main' layer

Notes on game mode:
The game layer moves the enable/disable tap dance key to the other side of the board above the right arrow key. This is to avoid accidental presses during gaming. Game mode state is held in memory and won't be saved after the keyboard is unplugged.

### Lower (layer on hold)

Access lower planck style layer.

### Raise (layer on hold)

Access upper planck style layer.

### Copy

On Mac OSX: `Cmd` + `c`,
Otherwise: `Ctrl` + `c`

### Paste

On Mac OSX: `Cmd` + `v`,
Otherwise: `Ctrl` + `v`

### Util (layer toggle)

Toggle the util layer on/off by tapping twice on this key.

