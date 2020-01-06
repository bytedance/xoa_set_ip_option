

PORT := 8192


run_client_v4: client_v4
	./client_v4 192.168.1.2 $(PORT)

run_client_v6: client_v6
	./client_v6 2001::2 $(PORT)

client_v4: client_v4.cpp ../ip_opt/ip_opt.h ../ip_opt/ip_opt.c
	g++ -o $@  -I ../ip_opt $^

client_v6: client_v6.cpp ../ip_opt/ip_opt.h ../ip_opt/ip_opt.c
	g++ -o $@  -I ../ip_opt $^
