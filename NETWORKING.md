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
|                                     |    "username": "<username>",        |
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
|                                     |    "username": "<username>",        |
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
|                                     |    "players_ready": [               | (in the same order as the players)
|                                     |      false,                         | (i.e.: username1 is not ready)
|                                     |      true                           | (i.e.: username2 is ready)
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
|                                    <--  Send to all players               |
|                                     |   {                                 |
|                                     |    "type": "ready",                 |
|                                     |    "player": "<username>",          |
|                                     |    "ready": true                    |
|                                     |   }                                 |
|                                     |   or                                |
|                                     |   {                                 |
|                                     |    "type": "ready",                 |
|                                     |    "player": "<username>",          |
|                                     |    "ready": false                   |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
|   Get games possibilities:         -->                                    |
|   {                                 |                                     |
|    "type": "games"                  |                                     |
|   }                                 |                                     |
|                                     |                                     |
|                                    <--  List all types of games           |
|                                     |   {                                 |
|                                     |    "type": "games",                 |
|                                     |    "games_name": [                  |
|                                     |      "<gameName1>",                 |
|                                     |      "<gameName2>"                  |
|                                     |    ],                               |
|                                     |    "games_description": [           | (int the same order as games_name)
|                                     |      "<description1>",              | (i.e.: description1 is the description of gameName1)
|                                     |      "<description2>"               | (i.e.: description2 is the description of gameName2)
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
|                                    <--  Start game                        |
|                                     |   to the owner requesting it        |
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
|                                     |   To all the participants (only     |
|                                     |   when success)                     |
|                                     |   {                                 |
|                                     |    "type": "start",                 |
|                                     |    "gameName": "<gameName>"         |
|                                     |   }                                 |
| ----------------------------------- | ----------------------------------- |
```
