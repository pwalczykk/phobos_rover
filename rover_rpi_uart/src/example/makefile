
all : ~/uart_example ~/uart_transmiter ~/uart_reciver ~/uart_transmiter_external ~/uart_reciver_external ~/uart_transmiter_external_19200 ~/uart_reciver_external_19200

~/uart_example : Example.cpp
	g++ -o $@ $^ -std=c++11

~/uart_transmiter : Transmiter.cpp
	g++ -o $@ $^ -std=c++11

~/uart_reciver : Reciver.cpp
	g++ -o $@ $^ -std=c++11

~/uart_transmiter_external : TransmiterExternal.cpp
	g++ -o $@ $^ -std=c++11

~/uart_transmiter_external_19200 : TransmiterExternal19200.cpp
	g++ -o $@ $^ -std=c++11

~/uart_reciver_external : ReciverExternal.cpp
	g++ -o $@ $^ -std=c++11

~/uart_reciver_external_19200 : ReciverExternal19200.cpp
	g++ -o $@ $^ -std=c++11
