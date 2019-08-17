# Custom DZ60 ISO MAC QERTY Layout

60% ISO key layout with customized Mac QUERY layout

Customizations:
- Shift + Backspace = Delete
- Caps Lock = Esc
- top left key (left of 1) = Esc on tap, or Fn on hold
- German umlauts are send as unicode when combined with a specific modifier key
- Vim style arrow keys below the right shift key
- Fn + first row = F1..F12 keys
- Mac style placement of modifier keys (ctrl, option, command)
- Fn + certain buttons in the second row = various media keys

## Build

```
git submodule update --init --recursive
make build
```

