## willko keymap for Idobao ID75 V2

All about color coded layers and the planck style layout with extra macros.

Macros are defined with mac osx in mind, but can be easily modified for windows if desired.

### Layout

![willko home layout](https://i.imgur.com/khxkgv7.png)


### Compile

```
make idobao/id75/v2:willko
```

or

```
qmk compile -kb idobao/id75/v2 -km willko
```

Note: The `idobao_id75_v2_willko.hex` output file should be in the `./.build` folder. 

### Flash

```
make idobao/id75/v2:willko:flash
```

or 

```
qmk flash -kb idobao/id75/v2 -km willko
```

