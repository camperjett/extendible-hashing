//Om Namah Shiva
#include <bits/stdc++.h>
using namespace std;

#define el '\n'

class Bucket
{
private:
	set<int> BUF;
	int SIZE;
	int LD;
public:
	Bucket(int ld, int size);
	int insert(int x);
	pair<Bucket,Bucket> split();
	int localDepth();
	set<int> records();
	bool isFull();
	void display();
	int remove(int x);
	void halfDIR();
	int count();
	void decrDepth();
};

class DIR
{
public:
	vector<Bucket*> Buckets;
	int GD;
	int BKT_SZ;

	DIR(int gd, int bkt_sz);
	void insert(int x, bool isReAssemble);
	void remove(int x, int mode);
	int HASH(int x);
	void split(int bkt_id);
	void doubleDIR();
	int complement(int bkt_id, int depth);
	void display();
	void merge(int bkt_id);
	void shrink();
};

DIR::DIR(int gd, int bkt_sz){
	this->GD = gd, this->BKT_SZ = bkt_sz;

	for (int i = 0; i < 1<<gd; ++i)
	{
		this->Buckets.push_back(new Bucket(gd, bkt_sz));
	}
}

int DIR::HASH(int x){
	return x & ((1<<GD)-1);
}

int DIR::complement(int bkt_id, int depth){
	return (1<<depth) ^ bkt_id;
}

void DIR::doubleDIR(){
	cout<<"-_- DOUBLE DIRECTORY"<<el;
	for (int i = 0; i < 1<<(this->GD); ++i)
	{
		(this->Buckets).push_back(Buckets[i]);
	}
	this->GD += 1;
}

void DIR::split(int bkt_id){
	cout<<":/ SPLIT BUCKET "<<bkt_id<<el;
	// 1
	int depth = Buckets[bkt_id]->localDepth();
	// 1 == 3 ? false
	if(depth == GD) {
		cout<<"-_- GLOBAL DEPTH == LOCALDEPTH"<<el;
		doubleDIR();
	}
	// 	01,
	//	11 (10 ^ 01 = 11)
	int MOD1 = bkt_id,
		MOD2 = complement(bkt_id, depth);
	// increase localDepth
	depth++;
	// Gives two buckets. now assign all pointers according
	// to their mod with 1<<(depth+1).
	// if mod == 01, then go with first bucket else
	// if mod == 11, go with second bucket
	Bucket* Bucket1 = new Bucket(depth, this->BKT_SZ);
	Bucket* Bucket2 = new Bucket(depth, this->BKT_SZ);
	Bucket* old = Buckets[bkt_id];
	// assign bucket address to different indices
	for (int i = 0; i < 1<<GD; ++i)
	{
		if(i%(1<<(depth))==MOD1) Buckets[i] = Bucket1;
		else if(i%(1<<(depth))==MOD2) Buckets[i] = Bucket2;
	}
	// now move them records
	for(auto&I: old->records()){
		this->insert(I, 1);
	}
	// delete old Bucket too
	delete old;
}
// mode is set when moving the records to new bucket
void DIR::insert(int x, bool isReAssemble){
	int Hash = HASH(x);
	int status_code = this->Buckets[Hash]->insert(x);

	switch(status_code){
		case 200:
			if(isReAssemble)
				cout<<":) MOVED "<<x<<" TO BUCKET "<<Hash<<el;
			else cout<<":) INSERT "<<x<<" IN BUCKET "<<Hash<<el;
			break;
		case 300:
			cout<<":( FOUND DUPLICATE FOR "<<x<<el;
			break;
		case 500:
			cout<<":| BUCKET "<<Hash<<" FULL "<<el;
			split(Hash);
			insert(x, 0);
			break;
		default:
			break;
	}
	return;

}

void DIR::display(){
	cout<<"GLOBAL DEPTH "<<this->GD<<el;
	for (int i = 0; i < 1<<(this->GD); ++i)
	{
		cout<<"NO: "<<i<<" ---";
		Buckets[i]->display();
	}
}

void DIR::remove(int x, int mode){
	int Hash = HASH(x);
	int status_code = this->Buckets[Hash]->remove(x);
	if(status_code == 200)
		cout<<":) REMOVED "<<x<<" FROM BUCKET "<<Hash<<el;
	else if(status_code == 400){
		cout<<":( CANNOT REMOVE "<<x<<" , NOT PRESENT"<<el;
		return;
	}
	if(mode>0){
		if((this->Buckets[Hash]->count() == 0) && (this->Buckets[Hash]->localDepth()>=1)){
			merge(Hash);
		}
	}
	if(mode>1 && this->GD > 0) shrink();
}

void DIR::merge(int bkt_id){

	int depth = this->Buckets[bkt_id]->localDepth();
	int sister = this->complement(bkt_id, depth-1);
	cout<<"XD MERGE BUCKETS "<<bkt_id<<" "<<sister<<el;
	int index_diff = 1<<depth;
	if(this->Buckets[sister]->localDepth() == depth){
		this->Buckets[sister]->decrDepth();
		delete(this->Buckets[bkt_id]);
		this->Buckets[bkt_id] = this->Buckets[sister];

		for(int i=bkt_id-index_diff ; i>=0 ; i-=index_diff )
            this->Buckets[i] = this->Buckets[sister];
        for(int i=bkt_id+index_diff ; i<(1<<(this->GD)) ; i+=index_diff )
            this->Buckets[i] = this->Buckets[sister];
	}

}

void DIR::shrink()
{
    int i;
    for( i=0 ; i<(this->GD) ; i++ )
    {
        if(this->Buckets[i]->localDepth()==this->GD)
        {
            return;
        }
    }
    this->GD--;
    for(i = 0 ; i < 1<<GD ; i++ )
        this->Buckets.pop_back();

	cout<<"XD SHRINK THE DIRECTORY BY HALF"<<el;
}


/**BUCKET**/

Bucket::Bucket(int ld, int size){
	this->LD = ld, this->SIZE = size;
}
int Bucket::localDepth(){
	return this->LD;
}
bool Bucket::isFull(){
	return this->SIZE == (this->BUF).size();
}

int Bucket::insert(int x){
	if((this->BUF).find(x) == (this->BUF).end()){
		if(isFull()) 
			return 500;
		(this->BUF).insert(x);
		return 200;
	} else return 300;
}

set<int> Bucket::records(){
	return this->BUF;
}
void Bucket::display(){
	cout<<" LOCAL DEPTH "<<this->LD<<el;
	for(auto& I: this->BUF){
		cout<<"| "<<I<<el;
	}
	cout<<el;
}
int Bucket::remove(int x){
	if((this->BUF).find(x) == (this->BUF).end()) return 400;
	else{
		(this->BUF).erase(x);
		return 200;
	}
}

int Bucket::count(){
	return this->BUF.size();
}
void Bucket::decrDepth(){
	this->LD--;
}

void showMenu(){
	cout<<"\nUwU Available options : \n";
	cout<<"* insert <value>"<<el;
	cout<<"* exit"<<el;
	cout<<"* display"<<el;
	cout<<"* remove <value>"<<el;
	cout<<el;
}

int main()
{
    ios_base::sync_with_stdio(0);

    int bucket_size, init_GD;
    cout<<"Set Bucket size : "; cin>>bucket_size;
    cout<<"Initial global depth : "; cin>>init_GD;
    	
    DIR EH(init_GD, bucket_size);
    cout<<"I made a directory for you!\nYou can do following queries\n";
    showMenu();
    string choice = "";
    int x;
    int value;
    do{
    	cout<<"> ";
    	cin>>choice;
    	cout<<el;
    	if(choice == "insert"){
			cin>>value;
			EH.insert(value, false);
    	} else if(choice == "display"){
    		EH.display();
    	} else if(choice == "remove"){
    		int mode;
    		cin>>value;
    		EH.remove(value, 2);
    	} else if(choice != "exit"){
    		cout<<"*U* INVALID! PLEASE SELECT FROM OPTIONS";
    		showMenu();
    	}

    } while(choice != "exit");

    return 0;
  	
}
