#!/bin/bash

./Ben_AFK localhost 4242 << EOF
/login test test
/list -a
/help
/list
/msg a : Hey !
Hello world
/quit
EOF