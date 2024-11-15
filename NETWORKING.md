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
|    "roomName": "<roomName>",        |                                     |
|    "username": "<username>"         |                                     |
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
|    "type": "create",                |                                     |
|    "username": "<username>"         |                                     |
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
|   Get Current State                -->                                    |
|   {                                 |                                     |
|    "type": "state"                  |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  Return State (the current user    |
|                                     |   is in the players list)           |
|                                     |   {                                 |
|                                     |    "type": "state",                 |
|                                     |    "players": [                     |
|                                     |      "<username1>",                 |
|                                     |      "<username2>"                  |
|                                     |    ],                               |
|                                     |    "owner": "<usernameX>"           |
|                                     |    "game": "<gameName>"             |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
|                                    <--  New player connected              |
|                                     |   {                                 |
|                                     |    "type": "newPlayer",             |
|                                     |    "player": "<username>"           |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
|                                    <--  A player disconnected             |
|                                     |   {                                 |
|                                     |    "type": "delPlayer",             |
|                                     |    "player": "<username>"           |
|                                     |   }                                 |
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
|                                    <--  List all types of games           |
|                                     |   {                                 |
|                                     |    "type": "games",                 |
|                                     |    "games": [                       |
|                                     |      {                              |
|                                     |       "name": "<gameName>",         |
|                                     |       "description": "<desc>"       |
|                                     |      }                              |
|                                     |    ]                                |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
|   Set Game                         -->                                    |
|   {                                 |                                     |
|    "type": "select",                |                                     |
|    "game": "<gameName>"             |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  Set Game                          |
|                                     |   To the owner requesting it        |
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
|                                     |   To all the participants (only     |
|                                     |   when success)                     |
|                                     |   {                                 |
|                                     |    "type": "select",                |
|                                     |    "gameName": "<gameName>"         |
|                                     |   }                                 |
|                                     |                                     |
| ----------------------------------- | ----------------------------------- |
|   Start Game                       -->                                    |
|   {                                 |                                     |
|    "type": "start",                 |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  Send start to all players         |
|                                     |   {                                 |
|                                     |    "type": "start",                 |
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
