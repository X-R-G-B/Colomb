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
    "topLeftX": <x>,                        |
    "topLeftY": <y>,                        |
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
    "topLeftX": <x>,                        |
    "topLeftY": <y>,                        |
    "image": "<identifier>",                |
    "visible": <true/false>,                |
    "clickable": <true/false>,              |
}                                           |
```

### buttonText

```
{                                           |
    "type": "button_text",                  |
    "topLeftX": <x>,                        |
    "topLeftY": <y>,                        |
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "text": "<text>",                       |
    "visible": <true/false>,                |
    "clickable": <true/false>,              |
}                                           |
```

### textEntry

```
{                                           |
    "type": "text_entry",                   |
    "topLeftX": <x>,                        |
    "topLeftY": <y>,                        |
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "placeholder": "<text>",                |
}                                           |
```

### popup

```
{                                           |
    "topLeftX": "<x>",                      |
    "topLeftY": "<y>",                      |
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "text": "<text>",                       |
    "choices": {                            |
        "<key>": "<text>",                  | When the <text> is clicked ...
        ...                                 | ... the key will be sent
    }                                       |
}                                           |
```
