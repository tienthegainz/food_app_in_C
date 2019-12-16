# food_app_in_C
- Read README in each folder to know what to do
- Install [cJSON](https://github.com/DaveGamble/cJSON.git)<br>
  (Recommend install via Cmake)
- Run:
    - `python3 database.py` &nbsp; &nbsp;to create database
    - `make` &nbsp; &nbsp; to make all necessary executor
    - `cd server && ./server {SERVER_PORT}` to run searching server
    - `cd restaurant && ./r_server {SERVER_PORT}` to run restaurant server *(must be a port existed inside database)*
    - `cd client && ./server {HOST} {SERVER_PORT}` to run client

