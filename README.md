# CppSolyanka
First project on c++

---

___Installing dependencies___
```
sudo apt update
sudo apt install cmake
```

___Buil project___
```
cd CppSolyanka
mkdir build && cd build
cmake ..
make -j
```

___Server start___
```
cd CppSolyanka/build/src/server
./server
```

___Client start___
```
cd CppSolyanka/build/src/server
./client
```
__Commands___

|                                  Commnad                                 |              Description             |
|--------------------------------------------------------------------------|--------------------------------------|
| registration <login> <name> <sername> <patranimyc> <pasport> <password>  | registering a new account            |
| login <login> <password>                                                 | authorization                        |
| create_bank_account                                                      | create a new bank account            |
| create_card <bank_acc_id>                                                | create a new bank card               |
| get_bank_accounts                                                        | return list with user bank accounts  |
| get_cards                                                                | return list with user cards          |
| get_balance <card_id>                                                    | return user balnce on a bank account |
| replenish_balance <card_id> <sum>                                        | put money into the account           |
| debit_funds <card_id> <sum>                                              | debits money from account            |
