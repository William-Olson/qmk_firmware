# disale mouse keys
MOUSEKEY_ENABLE = no

# enable tap dance support
TAP_DANCE_ENABLE = yes

# enable link time optimization for reducing firmware size
LTO_ENABLE=yes

# Disable locked mode in Vial
VIAL_INSECURE = yes

# include/link our rgb helper code
SRC += ../../../../idobao/id75/keymaps/willko/willko_rgb_helper.c
