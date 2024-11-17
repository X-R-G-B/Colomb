## Root strucure

```
{                                           |
    "name": "<configName>",                 |
    "assets": {                             |
        "<identifier>": "<url_path>",       |
        ...                                 |
    },                                      |
    "page": {                               |
        "<identifier>": {                   |
            "type": "<type>",               |
            ...                             |
        },                                  |
        ...                                 |
    },                                      |
    "popups": {                             |
        "<identifier>": {                   |
            ...                             |
        },                                  |
        ...                                 |
    }                                       |
}                                           |
```

## Type structure

**<type>**: div/buttonImage/buttonText/textEntry
**<identifier>**: unique text identifier
**<x>**: float, distance from parent
**<y>**: float, distance from parent
**<r>**: 0-255, red part of RGBA
**<g>**: 0-255, green part of RGBA
**<b>**: 0-255, blue part of RGBA
**<a>**: 0-255, alpha part of RGBA

### div

```
{                                           |
    "type": "div",                          |
    "topLeftX": <x>,                        | Distance x from parent
    "topLeftY": <y>,                        | Distance y from parent
    "childrens": {                          |
        "<identifier>": {                   |
            "type": "<type>",               |
            ...                             |
        },                                  |
        ...                                 |
    }                                       |
}                                           |
```

### buttonImage

```
{                                           |
    "type": "button_image",                 |
    "topLeftX": <x>,                        | Distance x from parent
    "topLeftY": <y>,                        | Distance y from parent
    "image": "<identifier>",                |
    "visible": <true/false>,                | Is it draw to screen
    "clickable": <true/false>,              | Is it clickable (it can be ...
                                            | ... invisible but still be ...
                                            | ... clicked)
}                                           |
```

### buttonText

```
{                                           |
    "type": "button_text",                  |
    "topLeftX": <x>,                        | Distance x from parent
    "topLeftY": <y>,                        | Distance y from parent
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "text": "<text>",                       |
    "visible": <true/false>,                | Is it draw to screen
    "clickable": <true/false>,              | Is it clickable (it can be ...
                                            | ... invisible but still be ...
                                            | ... clicked)
}                                           |
```

### textEntry

```
{                                           |
    "type": "text_entry",                   |
    "topLeftX": <x>,                        | Distance x from parent
    "topLeftY": <y>,                        | Distance y from parent
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "placeholder": "<text>",                | Text displayed before the ...
                                            | ... client enter some text
}                                           |
```

### popup

```
{                                           |
    "topLeftX": <x>,                        | Distance x from 0 (window)
    "topLeftY": <y>,                        | Distance y from 0 (window)
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "text": "<text>",                       |
    "choices": {                            |
        "<key>": "<text>",                  | When the <text> is clicked ...
                                            | ... the key will be sent
        ...                                 |
    }                                       |
}                                           |
```
