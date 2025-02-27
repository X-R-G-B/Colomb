## Root strucure

```
{                                           |
    "name": "<configName>",                 |
    "assets": {                             |
        "<identifier>": "<url_path>",       |
        ...                                 |
    },                                      |
    "page": [                               |
        {                                   |
            "id": "<identifier>",           |
            "type": "<type>",               |
            ...                             |
        },                                  |
        ...                                 |
    ],                                      |
    "popups": {                             |
        "<identifier>": {                   |
            ...                             |
        },                                  |
        ...                                 |
    }                                       |
}                                           |
```

## Type structure

- `<type>`:
    div/buttonImage/buttonText/textEntry
- `<identifier>`:
    unique text identifier
- `<x>`:
    float, distance from parent
- `<y>`:
    float, distance from parent
- `<r>`:
    0-255, red part of RGBA
- `<g>`:
    0-255, green part of RGBA
- `<b>`:
    0-255, blue part of RGBA
- `<a>`:
    0-255, alpha part of RGBA

### div

```
{                                           |
    "id": "<identifier>",                   |
    "type": "div",                          |
    "topLeftX": <x>,                        | Distance x from parent
    "topLeftY": <y>,                        | Distance y from parent
    "childrens": [                          |
        {                                   |
            "id": "<identifier>",           |
            "type": "<type>",               |
            ...                             |
        },                                  |
        ...                                 |
    ]                                       |
}                                           |
```

### button_image

```
{                                           |
    "id": "<identifier>",                   |
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

### button_text

```
{                                           |
    "id": "<identifier>",                   |
    "type": "button_text",                  |
    "topLeftX": <x>,                        | Distance x from parent
    "topLeftY": <y>,                        | Distance y from parent
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "fgSize": <f>,                          | Text size
    "text": "<text>",                       | Text
    "visible": <true/false>,                | Is it draw to screen
    "clickable": <true/false>,              | Is it clickable (it can be ...
                                            | ... invisible but still be ...
                                            | ... clicked)
}                                           |
```

### text_entry

```
{                                           |
    "id": "<identifier>",                   |
    "type": "text_entry",                   |
    "topLeftX": <x>,                        | Distance x from parent
    "topLeftY": <y>,                        | Distance y from parent
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "fgSize": <f>,                          | Text size
    "placeholder": "<text>",                | Text displayed before the ...
                                            | ... client enter some text
    "visible": <true/false>,                | Is it draw to screen
    "clickable": <true/false>,              | Is it clickable (it can't be ...
                                            | ... clicked if not visible
}                                           |
```

### popup

```
{                                           |
    "topLeftX": <x>,                        | Distance x from 0 (window)
    "topLeftY": <y>,                        | Distance y from 0 (window)
    "bgColor": [<r>, <g>, <b>, <a>],        | Background color
    "fgColor": [<r>, <g>, <b>, <a>],        | Text color
    "fgSize": <f>,                          | Text size
    "text": "<text>",                       | Text
    "choices": {                            |
        "<key>": "<text>",                  | When the <text> is clicked ...
                                            | ... the key will be sent
        ...                                 |
    },                                      |
    "visible": <true/false>                 | Is the popup shown (unlike ...
                                            | ... the buttons, when a popup ...
                                            | ... is not show, it can't be ...
                                            | ... clicked
}                                           |
```
