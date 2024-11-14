## Global Level
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
|                                     |    "success": true,                 |
|                                     |    "roomName": "<roomName>"         |
|                                     |   }                                 |
|                                     |   or                                |
|                                     |   {                                 |
|                                     |    "type": "join",                  |
|                                     |    "success": false                 |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
|   Create a game                    -->                                    |
|   {                                 |                                     |
|    "type": "create"                 |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  Send roomName (add the client to  |
|                                     |   the game                          |
|                                     |   {                                 |
|                                     |    "type": "create",                |
|                                     |    "success": true,                 |
|                                     |    "roomName": "<roomName>"         |
|                                     |   }                                 |
|                                     |   or                                |
|                                     |   {                                 |
|                                     |    "type": "create",                |
|                                     |    "success": false                 |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
```

## GamePending Level

```
|               Client                |                Server               |
| ----------------------------------- | ----------------------------------- |
|   Set Readyness                    -->  Set ready status of the client    |
|   {                                 |                                     |
|    "type": "ready",                 |                                     |
|    "ready": true                    |                                     |
|   }                                 |                                     |
|   or                                |                                     |
|   {                                 |                                     |
|    "type": "ready",                 |                                     |
|    "ready": false                   |                                     |
|   }                                 |                                     |
|                                     |                                     |
| ----------------------------------- | ----------------------------------- |
|   Get games possibilities:         -->                                    |
|   {                                 |                                     |
|    "type": "games"                  |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  List all games available for the  |
|                                     |   current number of person          |
|                                     |   {                                 |
|                                     |    "type": "games",                 |
|                                     |    "games": [                       |
|                                     |      {                              |
|                                     |       "name": "<gameName>",         |
|                                     |       "description": "<desc>"       |
|                                     |      }                              |
|                                     |     ]                               |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
|   Set Game                         -->                                    |
|   {                                 |                                     |
|    "type": "select"                 |                                     |
|    "game": "<gameName>"             |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  Set Game                          |
|                                     |   {                                 |
|                                     |    "type": "select",                |
|                                     |    "success": true,                 |
|                                     |    "gameName": "<gameName>"         |
|                                     |   }                                 |
|                                     |   or                                |
|                                     |   {                                 |
|                                     |    "type": "create",                |
|                                     |    "success": false                 |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
```
