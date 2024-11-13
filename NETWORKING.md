```
|               Client                |                Server               |
| ----------------------------------- | ----------------------------------- |
|   Connect (no message)             -->  Put the peer in a waiting         |
|                                     |   global lobby                      |
| ----------------------------------- | ----------------------------------- |
|   Connect to a pending game:       -->                                    |
|   {                                 |                                     |
|    "type": "join",                  |                                     |
|    "roomName": "<roomName>"         |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  Send connect ok with information  |
|                                     |   {                                 |
|                                     |    "type": "join",                  |
|                                     |    "success": true                  |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
|   Create a game                    -->                                    |
|   {                                 |                                     |
|    "type": "create"                 |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  Send roomName                     |
|                                     |   {                                 |
|                                     |    "type": "create",                |
|                                     |    "roomName": "<roomName>"         |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
```
