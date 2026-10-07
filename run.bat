@echo off

:: Build the simulator
g++ -std=c++20 -O2 -Wall -Wextra -Iinclude src\main.cpp -o vacuum_agent.exe

:: Build the tests
g++ -std=c++20 -O2 -Wall -Wextra -Iinclude tests\test_agents.cpp -o agent_tests.exe

:: Run them
agent_tests.exe
vacuum_agent.exe

pause

