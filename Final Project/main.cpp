#include<iostream>
#include<fstream>
using namespace std;

//Used to trim whitespace from a string
string Trim(string line){
	if(line.substr(0,1) == " "){
		line = line.erase(line.front());
	}
	
	if(line.substr(line.length(),1) == " "){
		line.pop_back();
	}
	return line;
}

//A huffman tree node
struct Huffman_Node{
	Huffman_Node* left;
	Huffman_Node* right;
	int frequency;
	string letter;
};

//used to store code generated by the huffman tree
string huffman_code = "";


//Used to generate a code based on the huffman tree
void generate_code(Huffman_Node* node, string code){
	
	//base case
	if(node->left == NULL && node->right == NULL){
		huffman_code += node->letter + code + "___";
	}
	
	
	if(node->left != NULL){
		generate_code(node->left, code + "0");
	}
	
	if(node->right != NULL){
		generate_code(node->right, code + "1");
	}
}

//Creating a node structure for the priority queue
struct Node{
	Node* next;
	int frequency;
	string letter;
	Huffman_Node* huffman_node;
};

bool empty(Node*head);

//Used for inserting data into a priority queue
void insert_node(Node* head, string letter, int frequency){
	Node* current = head;
	Node* temp = new Node();
	temp->huffman_node = NULL;
	temp->frequency = frequency;
	temp->letter = letter;
	
	while(current->next != NULL && frequency > current->next->frequency)
	{
		current = current->next;		
	}
	
	if(current->next == NULL){
		temp->next = NULL;
		current->next = temp;
	}
	else{
		temp->next = current->next;
		current->next = temp;
	}
	
}


// Used for inserting a node into the priority queue
void insert_node(Node* head, Node* node){
	Node* current = head;
	int frequency = node->frequency;
	
	while(current->next != NULL && frequency > current->next->frequency)
	{
		current = current->next;		
	}
	
	if(current->next == NULL){
		node->next = NULL;
		current->next = node;
	}
	else{
		node->next = current->next;
		current->next = node;
	}
	
}


// Used for inserting a huffman node into the priority queue
void insert_node(Node* head, Huffman_Node* node){
	Node* current = head;
	Node* temp = new Node();
	temp->huffman_node = node;
	temp->frequency = node->frequency;
	
	while(current->next != NULL && temp->frequency > current->next->frequency)
	{
		current = current->next;
	}
	
	if(current->next == NULL){
		temp->next = NULL;
		
		current->next = temp;
	}
	else{
		temp->next = current->next;
		current->next = temp;
	}
	
}


//Used for popping in a priority queue
Node* pop(Node* head){
	if(!empty(head)){
		Node* current = head;
		if(head->next->next != NULL){
			current = current->next;
			head->next = current->next;
			
			return current;
		}
		else if(head->next != NULL){
			current = head->next;
			head->next = NULL;
			return current;
		}
	}
}


//Used to start the priority queue
Node* initialize_queue(string letter, int frequency){
	Node* head = new Node();
	head->frequency = frequency;
	head->letter = letter;
	head->next = NULL;
	return head;
}


//Used to print every node in a priority queue
Node* printall(Node* head){
	Node* current = head;
	current = current->next;
	while(current != NULL){
		cout << current->letter;
		cout << current->frequency << endl;
		current = current->next;
		
	}
}


//Used to check if the queue is empty
bool empty(Node* head){
	if(head->next == NULL){
		return true;
	}
	return false;
}


void Compress(string filedir, string filesavedas){
	
	//Used to read the file
	ifstream filereader;
	
	//Used to convert string into index
	hash<string> myhash;
	
	//Used to temporarily store current items
	string current_char;
	int current_index;
	
	int modulus = 10000;
	
	//Used to store all letters used
	string letters[1000] = {""};
	
	//Used to count unique letters
	int letter_count = 0;
	
	//Used to count all letters
	int all_letter_count = -1;
	
	//Used to store frequency and letters
	int max_index = 10000;
	int letter_frequency[max_index] = {};
	string letter_present[max_index] = {""};
	
	//Opening of the file
	filereader.open(filedir);
	
	
	//Reading through the file char by char and adding into respective hash sets
	if(filereader.is_open()){
		while(filereader.good()){
			all_letter_count += 1;
			current_char = filereader.get();
			current_index = myhash(current_char) % modulus;
			
			//Adding the letter into the array if index of the array is null
			if(letter_present[current_index] == ""){
				letter_present[current_index] = current_char;
				letter_frequency[current_index] = 1;
				
				letters[letter_count] = current_char;
				letter_count += 1;
			}
			
			//Collision Prevention
			else if(letter_present[current_index] != current_char){
				
				//Moving one by one to find a free slot or the current char
				while(letter_present[current_index] != "" && letter_present[current_index] != current_char){
					if(current_index + 1 > max_index){
						current_index = current_index - max_index;
					}
					current_index = current_index + 1;
				}
				if(letter_present[current_index] == ""){
					letter_present[current_index] = current_char;
					letter_frequency[current_index] = 1;
					
					letters[letter_count] = current_char;
					letter_count += 1;
				}
				else{
					letter_frequency[current_index] += 1;
				}
			}
			
			//Increamenting frequency if same char is found
			else{
				letter_frequency[current_index] += 1;
			}
		}
	}
	cout << "LOG: hashing completed" << endl;
	filereader.close();
	
	Node* head = initialize_queue("",0);
	
	// Adding all of the data into the priority queue
	for(int x = 0; x < letter_count; x++)
	{
		current_index = myhash(letters[x]) % modulus;
		insert_node(head, letter_present[current_index], letter_frequency[current_index]);
		
	}
	
	//Creating Huffman Tree
	Node* left;
	Node* right;
	
	Huffman_Node *huffman_node, *leaf_left, *leaf_right;
	
	//removing something?
	pop(head);
	letter_count -= 1;

	while(!empty(head)){
		
		//Popping two nodes from queue
		left = pop(head);
		if(right = pop(head)){
			
			
			//Creating new huffman node
			huffman_node = new Huffman_Node();
			huffman_node->frequency = left->frequency + right->frequency;
			huffman_node->letter = "";
			
			//Creating left leaf
			if(left->huffman_node == NULL){
				leaf_left = new Huffman_Node();
				leaf_left->frequency = left->frequency;
				leaf_left->letter = left->letter;
				leaf_left->left = NULL;
				leaf_left->right = NULL;
				huffman_node->left = leaf_left;
			}
			else{
				huffman_node->left = left->huffman_node;
			}
			
			//Creating right leaf
			if(right->huffman_node == NULL){
				leaf_right = new Huffman_Node();
				leaf_right->frequency = right->frequency;
				leaf_right->letter = right->letter;
				leaf_right->left = NULL;
				leaf_right->right = NULL;
				huffman_node->right = leaf_right;
			}
			else{
				huffman_node->right = right->huffman_node;
			}
			
			insert_node(head, huffman_node);
		}
		
	}
	
	//Generating code from the huffman tree
	generate_code(left->huffman_node, "");
	//Removing last delimeter
	huffman_code.pop_back();
	huffman_code.pop_back();
	huffman_code.pop_back();
	
	
	//Creating an array of string to store the code
	string huffman_code_array[letter_count];
	
	//integer to be used to store maximum length of a code
	int max_code_length = 0;
	
	//Copy used to split code into array.
	string huffman_code_copy = huffman_code;
	
	
	//Position used to store where the split will occur
	int position;
	
	//Storing tokens into array
	for(int x = 0; x < letter_count; x++){
		if(x < letter_count - 1){
			position = huffman_code_copy.find("___");
		
			huffman_code_array[x] = huffman_code_copy.substr(0,huffman_code_copy.find("___"));
			huffman_code_copy.erase(0, position + 3);
			
			//Comparing length of string to get max length
			if(huffman_code_array[x].length() > max_code_length){
				max_code_length = huffman_code_array[x].length();
			}
		}
		else{
			huffman_code_array[x] = huffman_code_copy;
			if(huffman_code_array[x].length() > max_code_length){
				max_code_length = huffman_code_array[x].length();
			}
		}
	}
	
	//Clearing old huffman code
	huffman_code = "";
	
	// Adding 0 to code below max length
	for(int x = 0; x < letter_count; x++){
		
		if(huffman_code_array[x].length() < max_code_length){
			string zeroes_to_be_added((max_code_length - huffman_code_array[x].length()), '0');
			huffman_code_array[x] = huffman_code_array[x] + zeroes_to_be_added;
		}
		huffman_code += huffman_code_array[x] + "___";
	}
	huffman_code = Trim(huffman_code);
	
	cout << "LOG: Huffman completed" << endl;

	
	//Output file
	ofstream file_code_writer;
	ofstream file_compressed_writer;
	
	filereader.open(filedir);
	file_code_writer.open(filedir+"_code.txt");
	
	//Writing the code, letter count and code length into a file
	file_code_writer << max_code_length << " " << all_letter_count << " ";
	file_code_writer.flush();
	for(int x = 0; x < letter_count; x++){
		file_code_writer << huffman_code.substr(x * (max_code_length+5), max_code_length +5) ;
		file_code_writer.flush();
	}
	file_code_writer.close();
	
	file_compressed_writer.open(filesavedas);
	
	//Used to store temporary code
	string temp_code;
	
	//Used for comparison
	string zero = "0";
	
	//Used to get number of binary created
	float binary_count = 0;
	
	if(filereader.is_open()){
		
		for(int x = 0; x < all_letter_count; x++){
			current_char = filereader.get();
			temp_code = huffman_code.substr(huffman_code.find(current_char) + 1, max_code_length - 1);
			for(int i = 0; i < temp_code.length(); i++){
				
				if(string(1, temp_code[i]) == zero){
					file_compressed_writer << "0";
				}
				else{
					file_compressed_writer << "1";
				}
				file_compressed_writer.flush();
				binary_count += 1;
			}
		}
	}
	

	filereader.close();
	file_compressed_writer.close();
	
	cout << "File size: " << all_letter_count * 8 << " bits" << endl;
	cout << "Theoretical compressed file size: " << binary_count << " bits" << endl;
	cout << "Theoretical percentage space saved: " << 100 - binary_count/(all_letter_count * 8) * 100 << "%" << endl;
}


//Function to decompress
void Decompress(string filedir, string filecode, string filesavedas){
	ifstream code_filereader;
	ifstream main_filereader;
	ofstream filewriter;
	
	string huffman_code = "";
	string current;
	int max_code_length, all_letter_count;
	
	code_filereader.open(filecode);
	
	if(code_filereader.is_open()){
		
		//Get max length of code
		current = code_filereader.get();
		while((current) != " "){
				huffman_code += current;
				current = code_filereader.get();
		}
		max_code_length = stoi(huffman_code);
		huffman_code = "";
		
		current = code_filereader.get();
		while((current) != " "){
				huffman_code += current;
				current = code_filereader.get();
		}
		all_letter_count = stoi(huffman_code);
		huffman_code = "";
		
		while(code_filereader.good()){
			huffman_code += code_filereader.get();
		}
		
	}
	huffman_code.pop_back();
	cout << "LOG: code file read." << endl;
	
	code_filereader.close();
	
	
	main_filereader.open(filedir);
	filewriter.open(filesavedas);
	string current_code = "";
	if(main_filereader.is_open()){
		for(int i = 0; i < all_letter_count; i++){
			
			current_code = "";
			for(int x = 0; x < max_code_length - 1; x++){
				current_code += main_filereader.get();
			}
			if((huffman_code.find(current_code)-1) < huffman_code.length()){
				filewriter << huffman_code.substr(huffman_code.find(current_code)-1, 1);
				filewriter.flush();
			}
			
		}
	}
	
	main_filereader.close();
	filewriter.close();
};

int main(){
	Compress("test.txt", "test_compressed.txt");
	
	Decompress("test_compressed.txt", "test.txt_code.txt", "test_output.txt");
}