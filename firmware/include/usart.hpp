#pragma once

namespace uart
{
class Uart
{
public:
	Uart ( int ch, int bd, bool doinit );
	~Uart ( void );
	void send ( char c );
	char get ( void );
	void getnmea ( char* buffer );
	void print ( char ch );
	void print ( int num );
	void print ( char const* str );
private:
	void init ( void );
	int channel;
	int baud;
	char buf[11];
	USART_TypeDef* Reg;
};
}
