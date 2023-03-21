
void SUBPreload(){
	set_reg(4,120);
	set_reg(6,40);
	set_reg(12,20);
	set_reg(8,35);
	set_reg(7,57);
	set_reg(14,11);
	set_reg(2,8);
	set_reg(17,0xFFFFFFFB);//-5
	set_reg(18,23);
	set_reg(13,3);
	set_reg(5,0xFFFFFFC6); //-58
	set_reg(3,24);
	set_reg(21,0xFFFFFF9F); //-97
	set_reg(19,0xFFFFFFD3); //-45
	set_reg(11,0xFFFFFF59); //-167
	set_reg(25,0xFFFFFFD6); //-42
	set_reg(15,0xFFFFFFBD); // -67
	set_reg(20,0xFFFFFCEF); // -785
};

void XORPreload(){
	set_reg(4,0xAAAAAAAA);
	set_reg(6,0x55555555);
	set_reg(12,0xABCDEF12);
	set_reg(8,0xFFFFFFFF);
	set_reg(7,0xFFFFFFFF);
	set_reg(14,0x811);
	set_reg(2,0x80000000);
	set_reg(17,0x00000000);
	set_reg(18,0x23);
	set_reg(13,0x0000FFFF);
	set_reg(5,0xFFFF0000);
	set_reg(3,0x8724);
	set_reg(21,0xDFDFDFDF);
	set_reg(19,0xCCCCCCCC);
	set_reg(11,0xEFBCDA89);
	set_reg(25,0xF123456);
	set_reg(15,0xABCDDCBA);
	set_reg(20,0x12344321);
};

void SRAIPreload(){
	set_reg(4,0xAAAAAAAA);
	set_reg(6,0x55555555);
	set_reg(12,0xABCDEF12);
	set_reg(8,0xFFFFFFFF);
	set_reg(7,0xFFFFFFFF);
	set_reg(14,0x811);
	set_reg(2,0x80000000);
	set_reg(17,0x00000000);
	set_reg(18,0x23);
	set_reg(13,0x0000FFFF);
	set_reg(5,0xFFFF0000);
	set_reg(3,0x8724);
	set_reg(21,0xDFDFDFDF);
	set_reg(19,0xCCCCCCCC);
	set_reg(11,0xEFBCDA89);
	set_reg(25,0xF123456);
	set_reg(15,0xABCDDCBA);
	set_reg(20,0x12344321);
};

void ORIPreload(){
	set_reg(4,0xAAAAAAAA);
	set_reg(6,0x55555555);
	set_reg(12,0xABCDEF12);
	set_reg(8,0xFFFFFFFF);
	set_reg(7,0xFFFFFFFF);
	set_reg(14,0x811);
	set_reg(2,0x80000000);
	set_reg(17,0x00000000);
	set_reg(18,0x23);
	set_reg(13,0x0000FFFF);
	set_reg(5,0xFFFF0000);
	set_reg(3,0x8724);
	set_reg(21,0xDFDFDFDF);
	set_reg(19,0xCCCCCCCC);
	set_reg(11,0xEFBCDA89);
	set_reg(25,0xF123456);
	set_reg(15,0xABCDDCBA);
	set_reg(20,0x12344321);
};

void ADDIPreload(){
	set_reg(4,0xAAAAAAAA);
	set_reg(6,0x55555555);
	set_reg(12,0xABCDEF12);
	set_reg(8,0xFFFFFFFF);
	set_reg(7,0xFFFFFFFF);
	set_reg(14,0x811);
	set_reg(2,0x80000000);
	set_reg(17,0xFFFFFF00);
	set_reg(18,0x23);
	set_reg(13,0xFFFFFFFF);
	set_reg(5,0xFFFF0000);
	set_reg(3,0x8724);
	set_reg(21,0xDFDFDFDF);
	set_reg(19,0xCCCCCCCC);
	set_reg(11,0xEFBCDA89);
	set_reg(25,0xF123456);
	set_reg(15,0xABCDDCBA);
	set_reg(20,0x12344321);
	set_reg(31,0xFFFFFFFF);
};

void LHUPreload(){
	set_reg(4,0xAAAAAAAA);
	set_reg(6,0x00000055);
	mem_write(0x5A9,0x23);
	mem_write(0x5AA,0x54);
	mem_write(0x5AB,0xFB);
	mem_write(0x5AC,0x67);
	mem_write(0x5AD,0x90);
	set_reg(12,0xABCDEF12);
	set_reg(8,0xFFFFFFFF);
	set_reg(7,0x0000FFFF);
	mem_write(0xFFFE,0x23);
	mem_write(0xFFFF,0x54);
	set_reg(14,0x811);
	set_reg(2,0x80000000);
	mem_write(0xFFE0,0x87);
	mem_write(0xFFE1,0x54);
	set_reg(17,0xFFFFFF00);
	set_reg(18,0x23);
	set_reg(13,0xFFFFFFFF);
	set_reg(5,0xFFFF0000);
	set_reg(3,0x8724);
	set_reg(21,0xDFDFDFDF);
	set_reg(19,0xCCCCCCCC);
	set_reg(11,0xEFBCDA89);
	set_reg(25,0xF123456);
	set_reg(15,0xABCDDCBA);
	set_reg(20,0x12344321);
	set_reg(31,0xFFFFFFFF);
};

void LBPreload(){
	set_reg(4,0xAAAAAAAA);
	set_reg(6,0x00000055);
	mem_write(0x5A9,0x23);
	mem_write(0x5AA,0xF4);
	mem_write(0x5AB,0xFB);
	mem_write(0x5AC,0x67);
	mem_write(0x5AD,0x90);
	set_reg(12,0xABCDEF12);
	set_reg(8,0xFFFFFFFF);
	set_reg(7,0x0000FFFF);
	mem_write(0xFFFE,0x23);
	mem_write(0xFFFF,0x54);
	set_reg(14,0x811);
	set_reg(2,0x80000000);
	mem_write(0xFFE0,0x87);
	mem_write(0xFFE1,0x54);
	set_reg(17,0xFFFFFF00);
	set_reg(18,0x23);
	set_reg(13,0xFFFFFFFF);
	set_reg(5,0xFFFF0000);
	set_reg(3,0x8724);
	set_reg(21,0xDFDFDFDF);
	set_reg(19,0xCCCCCCCC);
	set_reg(11,0xEFBCDA89);
	set_reg(25,0xF123456);
	set_reg(15,0xABCDDCBA);
	set_reg(20,0x12344321);
	set_reg(31,0xFFFFFFFF);
};

void BLTPreload(){
	set_reg(6,0x00000055);
	set_reg(21,0x000005AA);
	set_reg(2,0x0000FFFF);
	set_reg(7,0x0000FFFF);
	set_reg(4,0xFFFFFF00);
	set_reg(17,0xFFFFFE00);
	set_reg(31,0xFFFF0000);
	set_reg(5,0xFFFF00FF);
	set_reg(30,0x80000000);
	set_reg(8,0x7FFFFC1F);
	set_reg(16,0x7FFFFC1F);
	set_reg(1,0x7FFFFD0F);
	set_reg(24,0x0F123456);
	set_reg(25,0x0F1233AB);
	set_reg(23,0x12344321);
	set_reg(20,0x12344A98);
	set_reg(3,0xFFFFFFFF);
	set_reg(13,0xFFFFFFFE);
};

void JALPreload(){
	set_reg(4,0xAAAAAAAA);
	set_reg(6,0x00000055);
	mem_write(0x5A9,0x23);
	mem_write(0x5AA,0xF4);
	mem_write(0x5AB,0xFB);
	mem_write(0x5AC,0x67);
	mem_write(0x5AD,0x90);
	set_reg(12,0xABCDEF12);
	set_reg(8,0xFFFFFFFF);
	set_reg(7,0x0000FFFF);
	mem_write(0xFFFE,0x23);
	mem_write(0xFFFF,0x54);
	set_reg(14,0x811);
	set_reg(2,0x80000000);
	mem_write(0xFFE0,0x87);
	mem_write(0xFFE1,0x54);
	set_reg(17,0xFFFFFF00);
	set_reg(18,0x23);
	set_reg(13,0xFFFFFFFF);
	set_reg(5,0xFFFF0000);
	set_reg(3,0x8724);
	set_reg(21,0xDFDFDFDF);
	set_reg(19,0xCCCCCCCC);
	set_reg(11,0xEFBCDA89);
	set_reg(25,0xF123456);
	set_reg(15,0xABCDDCBA);
	set_reg(20,0x12344321);
	set_reg(31,0xFFFFFFFF);
};

void PREload(){
	//SUBPreload();
	//XORPreload();
	//SRAIPreload();
	//ORIPreload();
	//ADDIPreload();
	//LHUPreload();
	//LBPreload();
	//BLTPreload();
	//JALPreload();
	// set_reg(0x04,0xFFFF0000);//rs2
	// set_reg(0x08,0xFFFE0000);//rs1
	// set_reg(0x05,0x00321018);//rd
	// set_reg(0x18,0x00000004);//rs2
	// set_reg(0x06,0x00000002);//rs1
	// set_reg(0xF1,0x00321018);//rd
}
