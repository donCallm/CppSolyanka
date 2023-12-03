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
```
registration      - registering a new account
login             - authorization
create_bank_acc   - create a new bank account
create_card       - create a new bank card
get_bank_accounts - return list with user bank accounts
get_cards         - return list with user cards
get_balance       - return user balnce on a bank account
replenish_balance - put money into the account
debit_funds       - debits money from account

example:
  command         - registration <login> <name> <sername> <patranimyc> <pasport> <password>
  expected result - Successful registration
  command         - login <login> <password>
  expected result - Successful loged
  command         - create_bank_account
  expected result - Successful created new bank account
  command         - create_card <bank_acc_id>
  expected result - Successful created new card
  command         - get_bank_accounts
  expected result - Id ur bank accounts: ...
  command         - get_cards
  expected result - Id ur cards: ...
  command         - get_balance <card_id>
  expected result - balance
  command         - replenish_balance <card_id> <sum>
  expected result - Successful change balance
  command         - debit_funds <card_id> <sum>
```
