#ifndef _MYLIB_H_
#define _MYLIB_H_
#include"iostream"
using namespace std;
class share_int;
class share_char;
class share_double;
class share_float;
class share_intArray;
class share_charArray;
class share_doubleArray;
class share_floatArray;
class share_memory;

class share{
    private:	        
    public:
    int i;
    double d;
    float f;
    int array[2000];//share_int 数组
    float f_array[2000];
    double d_array[2000];
    int array_cpy;
    int array_cpy_size;//share_int 数组大小
    char s;
    char c_array[2000];//share_char 数组
    void *share_buff[2000];
    int array_id;
      
    share_int *p_int;
    share_double *p_double;
    share_float *p_float;
    share_char *p_char;
    share_intArray *p_intArray;
    share_doubleArray *p_doubleArray;
    share_floatArray *p_floatArray;
    share_charArray *p_charArray;
    share_memory *p_memory;
    int c_type; //传输用的，1 int;2 intArray;3 char;4 charArray;5 int[];6 char[],7 double;8 float;9 doubleArray;10 floatArray;11 d_A[];12f_A[];13 memory
    int share_type;//share中class的类型，1是int，2是char ,3是intArray，4是charArray 7 double;8 float;9 doubleArray;10 floatArray; 11 d_A[];12 f_A[];13 memory     
	int write_data;	
	int change_data;
	int local;
	int write_allow;
	int read;
	int write;
	int change;
	char name[256];
	int getValue(){
		int x = i;
		return x;
	}
	int writex(int x){
		i = x;
		return 0;
	}	
};
typedef struct node {
	int data;
	char name[512];
    share *p;
	struct node *next;
}LNode, *List;

class share_int{
	private:
	int i;
    char name[256];	        
    public:
    int write_allow;    
	int write_data;
	int local;
	share_int(){}
	share_int(int j){
		i=j;
	}
	int newValue();// 向主节点请求最新值
	int getValue(){
		int x = i;
		return x;
	}
	int writex(int x){
		i = x;
		return 0;
	}
	
	operator int(){
        
        return i;
           
	}
    int operator=(const int j);
    int operator=(const share_int share1);
    void create(const char *cname);
    void get(const char *cname);
    friend share_int operator+(const int &j,const share_int &share1);
    friend share_int operator+(const share_int &share1,const int &j);
    friend share_int operator-(const int &j,const share_int &share1);
    friend share_int operator-(const share_int &share1,const int &j);
    friend share_int operator+(const share_int &share1,const share_int &share2){
		share_int share3;
		share3.i=share1.i+share2.i;
		return share3;
	}
	friend share_int operator-(const share_int &share1,const share_int &share2){
		share_int share3;
		share3.i=share1.i-share2.i;
		return share3;
	}
	friend share_int operator*(const int &j,const share_int &share1);
    friend share_int operator*(const share_int &share1,const int &j);
    friend share_int operator*(const share_int &share1,const share_int &share2){
		share_int share3;
		share3.i=share1.i*share2.i;
		return share3;
	}
	friend share_int operator/(const int &j,const share_int &share1);
    friend share_int operator/(const share_int &share1,const int &j);
    friend share_int operator/(const share_int &share1,const share_int &share2){
		share_int share3;
		share3.i=share1.i/share2.i;
		return share3;
	}
    friend ostream & operator<<(ostream &out, share_int A){
		out << A.i;
		return out;
	}	
};
class share_char{
	private:	
	char s;
    char name[256];       
    public:
    int write_allow;    
	char c_write_data;
	int local;	

	share_char(){}
	share_char(char cs){
		s=cs;
	}
	char newValue();
	char getValue(){
		char x = s;
		return x;
	}
	char writex(char x){
		s = x;
		return s;
	}
	void create(const char *cname);
	void get(const char *cname);
	operator char(){ 
       
       return s;
	}
    char operator=(const char j);
    char operator=(const share_char share1);
    friend ostream & operator<<(ostream &out, share_char A){
		out << A.s;
		return out;
	}	
};
class share_double{
	private:
	double i;
    char name[256];	        
    public:
    int write_allow;    
	double write_data;
	int local;
	share_double(){}
	share_double(double j){
		i=j;
	}
	double newValue();// 向主节点请求最新值
	double getValue(){
		double x = i;
		return x;
	}
	double writex(double x){
		i = x;
		return i;
	}
	
	operator double(){
        
        return i;
           
	}
    double operator=(const double j);
    double operator=(const share_double share1);
    void create(const char *cname);
    void get(const char *cname);
    friend share_double operator+(const double &j,const share_double &share1);
    friend share_double operator+(const share_double &share1,const double &j);
    friend share_double operator-(const double &j,const share_double &share1);
    friend share_double operator-(const share_double &share1,const double &j);
    friend share_double operator+(const share_double &share1,const share_double &share2){
		share_double share3;
		share3.i=share1.i+share2.i;
		return share3;
	}
	friend share_double operator-(const share_double &share1,const share_double &share2){
		share_double share3;
		share3.i=share1.i-share2.i;
		return share3;
	}
	friend share_double operator*(const double &j,const share_double &share1);
    friend share_double operator*(const share_double &share1,const double &j);
    friend share_double operator*(const share_double &share1,const share_double &share2){
		share_double share3;
		share3.i=share1.i*share2.i;
		return share3;
	}
	friend share_double operator/(const double &j,const share_double &share1);
    friend share_double operator/(const share_double &share1,const double &j);
    friend share_double operator/(const share_double &share1,const share_double &share2){
		share_double share3;
		share3.i=share1.i/share2.i;
		return share3;
	}
    friend ostream & operator<<(ostream &out, share_double A){
		out << A.i;
		return out;
	}	
};
class share_float{
	private:
	float i;
    char name[256];	        
    public:
    int write_allow;    
	float write_data;
	int local;
	share_float(){}
	share_float(float j){
		i=j;
	}
	float newValue();// 向主节点请求最新值
	float getValue(){
		float x = i;
		return x;
	}
	float writex(float x){
		i = x;
		return i;
	}
	
	operator float(){
        
        return i;
           
	}
    float operator=(const float j);
    float operator=(const share_float share1);
    void create(const char *cname);
    void get(const char *cname);
    friend share_float operator+(const float &j,const share_float &share1);
    friend share_float operator+(const share_float &share1,const float &j);
    friend share_float operator-(const float &j,const share_float &share1);
    friend share_float operator-(const share_float &share1,const float &j);
    friend share_float operator+(const share_float &share1,const share_float &share2){
		share_float share3;
		share3.i=share1.i+share2.i;
		return share3;
	}
	friend share_float operator-(const share_float &share1,const share_float &share2){
		share_float share3;
		share3.i=share1.i-share2.i;
		return share3;
	}
	friend share_float operator*(const float &j,const share_float &share1);
    friend share_float operator*(const share_float &share1,const float &j);
    friend share_float operator*(const share_float &share1,const share_float &share2){
		share_float share3;
		share3.i=share1.i*share2.i;
		return share3;
	}
	friend share_float operator/(const float &j,const share_float &share1);
    friend share_float operator/(const share_float &share1,const float &j);
    friend share_float operator/(const share_float &share1,const share_float &share2){
		share_float share3;
		share3.i=share1.i/share2.i;
		return share3;
	}
    friend ostream & operator<<(ostream &out, share_float A){
		out << A.i;
		return out;
	}
};
class share_doubleArray{
	private:
	double array[2000];
	char name[256];
    public:
	int local;
	int write_allow;
	double* getPtr(){
		return array;
	}
	double wtArray(int x,double y){
		array[x]=y;
		return y;
	}
	void create(const char *cname);
	void get(const char *cname); 
	void set(int x,double y);
	double get(int x){

		return array[x];
	}
	void copy(int *m,int x);
};
class share_floatArray{
	private:
	float array[2000];
	char name[256];
    public:
	int local;
	int write_allow;
	float* getPtr(){
		return array;
	}
	float wtArray(int x,float y){
		array[x]=y;
		return y;
	}
	void create(const char *cname);
	void get(const char *cname); 
	void set(int x,float y);
	float get(int x){

		return array[x];
	}
	void copy(int *m,int x);
};
class share_intArray{
    private:
	int array[2000];
	char name[256];
    public:
	int local;
	int write_allow;
	int* getPtr(){
		return array;
	}
	int wtArray(int x,int y){
		array[x]=y;
		return y;
	}
	void create(const char *cname);
	void get(const char *cname); 
	void set(int x,int y);
	int get(int x){

		return array[x];
	}
	void copy(int *m,int x);
};
class share_charArray{
    private:
	char array[2000];
	char name[256];
    public:
	int local;
	int write_allow;
	char* getPtr(){
		return array;
	}
	int wtArray(int x,char y){
		array[x]=y;
		return y;
	}
	void create(const char *cname);
	void get(const char *cname); 
	void set(int x,char y);
	char get(int x){

		return array[x];
	}
	void copy(char *m,int x);
};
class share_memory{
    private:
    char name[256];	        
    public:
	char s[500];
	void *share_buff[2000];
	int local;
	int write_allow;
	void create(const char *cname);
	void get(const char *cname); 
	void copyfrom(void *s,int count);
	void copyto(void *s,int count);			
};
List share_start();
void share_end();
#endif
