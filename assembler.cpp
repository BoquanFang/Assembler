/**
 * Log:
 * Need more testings
 * Right adjustments
 * Negative Global
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cmath> 
#include <iomanip>

//global variables go here
static int byte = 0;
static std::vector<std::string> file_each_line;
static std::map<std::string, std::string> general; //all codes for all instrcutions
static std::vector<std::string> final_result_set_up;
static std::vector<std::string> final_result;
static std::vector<std::string> labels;

//Handle Input
int handle_input(std::string file_name) {
    std::ifstream file_input (file_name);
    if (file_input.fail()) {
        std::cerr << "Failed to open file." << std::endl;
        return 2;
    }
    std::string each_line;
    while (getline(file_input, each_line)) {
        file_each_line.push_back(each_line);
    }
    return 0;
}
//Converter helper function
std::string converter_10_2 (unsigned base_10) {
    std::string result;
    std::string final_result;
    unsigned temp1 = 0;
	unsigned temp2 = base_10;
    while (temp2 != 0) {
		temp1 = temp2 % 2;
		temp2 = temp2 / 2;
		result += std::to_string(temp1);
    }
    while (result.length() < 32) {
        result += std::to_string(0);
    }
    for (int i = result.length() - 1; i >= 0; i--) {
        final_result.push_back(result[i]);
    }

    return final_result;
	
}
//Set up general
void handle_general() {
    //CPU
    general.insert(std::pair<std::string, std::string>("R1","000"));
    general.insert(std::pair<std::string, std::string>("R2","001"));
    general.insert(std::pair<std::string, std::string>("R3","010"));
    general.insert(std::pair<std::string, std::string>("R4","011"));
    general.insert(std::pair<std::string, std::string>("R5","100"));
    general.insert(std::pair<std::string, std::string>("R6","101"));
    general.insert(std::pair<std::string, std::string>("RS","110"));
    general.insert(std::pair<std::string, std::string>("RB","111"));
    //Two operands instructions
    general.insert(std::pair<std::string, std::string>("mov","0000"));
    general.insert(std::pair<std::string, std::string>("add","0001"));
    general.insert(std::pair<std::string, std::string>("cmp","0010"));
    //One operand instructions
    general.insert(std::pair<std::string, std::string>("push","0011"));
    general.insert(std::pair<std::string, std::string>("pop","0100"));
    general.insert(std::pair<std::string, std::string>("call","0101"));
    general.insert(std::pair<std::string, std::string>("je","0110")); 
    general.insert(std::pair<std::string, std::string>("jge","0111"));
    general.insert(std::pair<std::string, std::string>("jl","1000"));
    general.insert(std::pair<std::string, std::string>("j","1001"));
    //zero operand instructions
    general.insert(std::pair<std::string, std::string>("ret","1010"));
    general.insert(std::pair<std::string, std::string>("nop","1011"));
    //addressing mode
    general.insert(std::pair<std::string, std::string>("Immediate","00"));
    general.insert(std::pair<std::string, std::string>("Register","01"));
    general.insert(std::pair<std::string, std::string>("Direct","10"));
    general.insert(std::pair<std::string, std::string>("Indexed","11"));
}
//is_number
bool is_number(std::string temp) {
    int i;
    if (temp.at(0) == '-') {
        i = 1;
    } else {
        i = 0;
    }
    for (; i < (int) temp.length(); i ++) {
        if (std::isdigit(temp.at(i))==0) {
            return false;
        }
    }
    return true;
}
//get Empty Space in a String
std::vector<int> index_of_empty_space(std::string temp) {
    std::vector<int> result;
    for (int i = 4; i < (int) temp.length(); i ++) {
        if (temp.at(i) == ' ') {
            result.push_back(i);
        }
    }
    return result;
}
//contains or not in a vector
bool is_contain(std::string temp) {
    for (int i = 0; i < (int) labels.size(); i ++) {
        if (labels.at(i) == temp) {
            return true;
        }
    }
    return false;
}

//two's completment
std::string two_complement(int temp) {
    int add = 0;
    std::string given = converter_10_2(abs(temp));
    for (int i = 0 ; i < (int) given.length(); i ++) {
        if (given.at(i) == '0') {
            given.at(i) = '1';
        } else {
            given.at(i) = '0';
        }
    }
    std::string add_1 = "00000000000000000000000000000001";
    for (int i = 0 ; i < (int) given.length(); i ++) {
        add += (given.at(31 - i) - '0') + (add_1.at(31 - i) - '0');
        if (add < 2) {
            given[31 - i] = (char) (add + 48);
            add = 0;
        } else if (add == 2) {
            given[31 - i] = '0';
            add = 1;
        }
    }
    return given; 
}

//label's value
void set_up_labels() {
    //local variables
    //int count = 0;
    int length_file = (int) file_each_line.size(); 
    for (int i = 0; i < length_file; i ++) {
        if (file_each_line.at(i).at(0) != ' ') { //this takes care of the address of labels
            general.insert(std::pair<std::string, std::string>(file_each_line.at(i),std::to_string(byte)));
            labels.push_back(file_each_line.at(i));
            //std::cout << "We are appending " << file_each_line.at(i) << " with value of " << byte << std::endl;
        } else if (file_each_line.at(i).substr(4,3) == "var") { //this will be the case for global variables
            int length_line = (int) file_each_line.at(i).length();
            int temp = (int) atoi(file_each_line.at(i).substr(8,  length_line - 8 ).c_str() );
            std::string base_2;
            if (temp < 0) {
                base_2 = two_complement(temp);
            } else {
                base_2 = converter_10_2(temp);
            }
            final_result_set_up.push_back(base_2.substr(0,8));
            final_result_set_up.push_back(base_2.substr(8,8));
            final_result_set_up.push_back(base_2.substr(16,8));
            final_result_set_up.push_back(base_2.substr(24,8));
            byte += 4;
        } else { // decoding instructions from here
            std::string temp;


            std::vector<int> index_space = index_of_empty_space(file_each_line.at(i));
            //std::cout << "The length of the index string is " << index_space.length() << std::endl;
            if ( (int) index_space.size() == 2) {//Two operands operation
                std::string cur_instruction = file_each_line.at(i).substr(4,3);
                temp.append(general[cur_instruction]);
                //determine first operand's addressing mode and value
                if (file_each_line.at(i).at((int)index_space.at(0)+1) == '$') {//imediate mode
                    temp.append("00");  
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+2, (int)index_space.at(1) - (int) index_space.at(0) - 1);
                    if (is_number(first)) { // if this is a number
                        if (atoi(first.c_str()) < 0) {
                            temp.append(two_complement(atoi(first.c_str())));
                        } else {
                            temp.append(converter_10_2(atoi(first.c_str())));
                        }

                    } else if (is_contain(first)) {
                        temp.append(converter_10_2(atoi(general[first].c_str())));
                    } else {
                        temp.append("00000000000000000000000000000000");
                    }

                } else if (file_each_line.at(i).at((int)index_space.at(0)+1) == 'R') {//Register Mode
                    temp.append("01");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, (int)index_space.at(1) - (int) index_space.at(0) - 1);
                    temp.append(general[first]);
                } else if (file_each_line.at(i).at((int)index_space.at(1)-1) == ')') {//Indexed Mode
                    temp.append("11");
                    std::string first_reg = file_each_line.at(i).substr((int)index_space.at(1)-3, 2);     
                    std::string first_num = file_each_line.at(i).substr((int)index_space.at(0)+1, (int)index_space.at(1)-4 - ((int)index_space.at(0)+1)); 
                    if (atoi(first_num.c_str()) < 0) {
                        temp.append(two_complement(atoi(first_num.c_str())));
                    } else {
                        temp.append(converter_10_2(atoi(first_num.c_str())));
                    }

                    temp.append(general[first_reg]);        
                } else { // Direct Mode
                    temp.append("10");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, (int)index_space.at(1) - (int) index_space.at(0) - 1);
                    if (is_contain(first)) {
                        temp.append(converter_10_2(atoi(general[first].c_str())));
                    } else {
                        temp.append("00000000000000000000000000000000");                       
                    }

                }
                //Now do the second one
                int length = (int) file_each_line.at(i).length();
                if (file_each_line.at(i).at((int)index_space.at(1)+1) == '$') {//imediate mode
                    temp.append("00");
                    std::string second = file_each_line.at(i).substr((int)index_space.at(1)+2, length - (int) index_space.at(1) - 2);
                    if (is_number(second)) { // if this is a number
                        if (atoi(second.c_str()) < 0) {
                            temp.append(two_complement(atoi(second.c_str())));
                        } else {
                            temp.append(converter_10_2(atoi(second.c_str())));
                        }

                    } else if (is_contain(second)) {
                        temp.append(converter_10_2(atoi(general[second].c_str())));
                    } else {
                        temp.append("00000000000000000000000000000000"); 
                    }
                } else if (file_each_line.at(i).at((int)index_space.at(1)+1) == 'R') {//Register Mode
                    temp.append("01");
                    std::string second = file_each_line.at(i).substr((int)index_space.at(1)+1, length - (int) index_space.at(1) - 2);
                    temp.append(general[second]);
                } else if (file_each_line.at(i).at(length-1) == ')') {//Indexed Mode
                    temp.append("11");
                    std::string second_reg = file_each_line.at(i).substr(length-3, 2);     
                    std::string second_num = file_each_line.at(i).substr((int)index_space.at(1)+1, length - 4 - ((int)index_space.at(1)) - 1 );  
                    if (atoi(second_num.c_str()) < 0) {
                        temp.append(two_complement(atoi(second_num.c_str())));
                    } else {
                        temp.append(converter_10_2(atoi(second_num.c_str()))); 
                    }
                    temp.append(general[second_reg]);            
                } else { // Direct Mode
                    temp.append("10");
                    std::string second = file_each_line.at(i).substr((int)index_space.at(1)+1, length - (int) index_space.at(1) - 1);
                    if (is_contain(second)) {
                        temp.append(converter_10_2(atoi(general[second].c_str())));
                    } else {
                        temp.append("00000000000000000000000000000000"); 
                    }

                }
                //Add zeros make it a multiple of eight and push back
                //std::cout << "Before adding zeros, temp is " << temp << std::endl;
                int final_length = (int) temp.length();
                //std::cout << "Before adding zeros, the length of temp is " << final_length << std::endl;
                int left_over = final_length % 8;
                //std::cout << "The left over is " << left_over << std::endl;
                while (left_over != 8 && left_over != 0) {
                    temp.append("0");
                    left_over ++;
                }
                final_length = (int) temp.length(); //Not a typo, but to update
                //std::cout << "After adding zeros, the length is " << final_length << std::endl;
                int num_of_bytes = (final_length / 8);
                byte += num_of_bytes;
                // Need to push back
                int i = 0;
                while (i < final_length) {
                    //std::cout << "Now we append " << temp.substr(i,8) << " while i is " << i << std::endl;
                    final_result_set_up.push_back(temp.substr(i,8));   
                    i += 8;                
                }
            } 



            if ((int) index_space.size() == 1) {// One operand operation
                int length_of_operand_code = index_space.at(0) - 4;
                std::string cur_instruction;
                if (length_of_operand_code == 4) {
                    cur_instruction = file_each_line.at(i).substr(4,4);
                } else if (length_of_operand_code == 3) {
                    cur_instruction = file_each_line.at(i).substr(4,3);
                } else if (length_of_operand_code == 2) {
                    cur_instruction = file_each_line.at(i).substr(4,2);
                } else {
                    cur_instruction = file_each_line.at(i).substr(4,1);
                }
                temp.append(general[cur_instruction]);
                int length = (int) file_each_line.at(i).length();
                if (file_each_line.at(i).at((int)index_space.at(0)+1) == '$') {//imediate mode
                    temp.append("00");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+2, length - (int) index_space.at(0) - 2);
                    if (is_number(first)) { // if this is a number
                        if (atoi(first.c_str()) < 0) {
                            temp.append(two_complement(atoi(first.c_str())));
                        } else {
                            temp.append(converter_10_2(atoi(first.c_str())));
                        }

                    } else if (is_contain(first)) {
                        temp.append(converter_10_2(atoi(general[first].c_str())));
                    } else {
                        temp.append("00000000000000000000000000000000");
                    }
                } else if (file_each_line.at(i).at((int)index_space.at(0)+1) == 'R') {//Register Mode
                    temp.append("01");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, length - (int) index_space.at(0) - 1);
                    temp.append(general[first]);
                } else if (file_each_line.at(i).at(length-1) == ')') {//Indexed Mode
                    temp.append("11");
                    std::string first_reg = file_each_line.at(i).substr(length-3, 2);     
                    std::string first_num = file_each_line.at(i).substr((int)index_space.at(0)+1, length - 4 - ((int)index_space.at(0)) - 1 );  
                    if (atoi(first_num.c_str()) < 0) {
                        temp.append(two_complement(atoi(first_num.c_str())));
                    } else {
                        temp.append(converter_10_2(atoi(first_num.c_str())));
                    }
                    temp.append(general[first_reg]);            
                } else { // Direct Mode
                    temp.append("10");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, length - (int) index_space.at(0) - 1);
                    if (is_contain(first)) {
                        temp.append(converter_10_2(atoi(general[first].c_str())));
                    } else {
                        temp.append("00000000000000000000000000000000"); 
                    }

                }
                int final_length = (int) temp.length();

                int left_over = final_length % 8;
                while (left_over != 8 && left_over != 0) {
                    temp.append("0");
                    left_over ++;
                }
                final_length = (int) temp.length(); //Not a typo, but to update
                int num_of_bytes = (final_length / 8);
                byte += num_of_bytes;
                // Need to push back
                int i = 0;
                while (i < final_length) {
                    final_result_set_up.push_back(temp.substr(i,8));   
                    i += 8;                
                }
            }
            
                
            if ((int) index_space.size() == 0) {//Zero Operand Operation
                std::string cur_instruction = file_each_line.at(i).substr(4,3);
                temp.append(general[cur_instruction]);
                int final_length = (int) temp.length();
                int left_over = final_length % 8;
                //std::cout << "The left over is " << left_over << std::endl;
                while (left_over != 8 && left_over != 0) {
                    temp.append("0");
                    left_over ++;
                }
                final_length = (int) temp.length(); //Not a typo, but to update
                int num_of_bytes = (final_length / 8);
                byte += num_of_bytes;
                // Need to push back
                int i = 0;
                while (i < final_length) {
                    final_result_set_up.push_back(temp.substr(i,8));   
                    i += 8;                
                }

            }       
                

        }

    }
}

//encoding
void encoding() {
    //local variables
    //int count = 0;
    int length_file = (int) file_each_line.size(); 
    for (int i = 0; i < length_file; i ++) {
        if (file_each_line.at(i).at(0) != ' ') { //this takes care of the address of labels
            //general.insert(std::pair<std::string, std::string>(file_each_line.at(i),std::to_string(byte)));
            //std::cout << "We are appending " << file_each_line.at(i) << " with value of " << byte << std::endl;
            continue;
        } else if (file_each_line.at(i).substr(4,3) == "var") { //this will be the case for global variables
            int length_line = (int) file_each_line.at(i).length();
            int temp = (int) atoi(file_each_line.at(i).substr(8,  length_line - 8 ).c_str() );
            std::string base_2;
            if (temp < 0) {
                base_2 = two_complement(temp);
            } else {
                base_2 = converter_10_2(temp);
            }
            final_result.push_back(base_2.substr(0,8));
            final_result.push_back(base_2.substr(8,8));
            final_result.push_back(base_2.substr(16,8));
            final_result.push_back(base_2.substr(24,8));
            //byte += 4;
        } else { // decoding instructions from here
            std::string temp;


            std::vector<int> index_space = index_of_empty_space(file_each_line.at(i));
            //std::cout << "The length of the index string is " << index_space.length() << std::endl;
            if ( (int) index_space.size() == 2) {//Two operands operation
                std::string cur_instruction = file_each_line.at(i).substr(4,3);
                temp.append(general[cur_instruction]);
                //determine first operand's addressing mode and value
                if (file_each_line.at(i).at((int)index_space.at(0)+1) == '$') {//imediate mode
                    temp.append("00");  
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+2, (int)index_space.at(1) - (int) index_space.at(0) - 1);
                    if (is_number(first)) { // if this is a number
                        if (atoi(first.c_str()) < 0) {
                            temp.append(two_complement(atoi(first.c_str())));
                        } else {
                            temp.append(converter_10_2(atoi(first.c_str())));
                        }

                    } else {
                        temp.append(converter_10_2(atoi(general[first].c_str())));
                    }

                } else if (file_each_line.at(i).at((int)index_space.at(0)+1) == 'R') {//Register Mode
                    temp.append("01");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, (int)index_space.at(1) - (int) index_space.at(0) - 1);
                    temp.append(general[first]);
                } else if (file_each_line.at(i).at((int)index_space.at(1)-1) == ')') {//Indexed Mode
                    temp.append("11");
                    std::string first_reg = file_each_line.at(i).substr((int)index_space.at(1)-3, 2);     
                    std::string first_num = file_each_line.at(i).substr((int)index_space.at(0)+1, (int)index_space.at(1)-4 - ((int)index_space.at(0)+1)); 
                    if (atoi(first_num.c_str()) < 0) {
                        temp.append(two_complement(atoi(first_num.c_str())));
                    } else {
                        temp.append(converter_10_2(atoi(first_num.c_str())));
                    }
                    temp.append(general[first_reg]);        
                } else { // Direct Mode
                    temp.append("10");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, (int)index_space.at(1) - (int) index_space.at(0) - 1);
                    temp.append(converter_10_2(atoi(general[first].c_str())));
                }
                //Now do the second one
                int length = (int) file_each_line.at(i).length();
                if (file_each_line.at(i).at((int)index_space.at(1)+1) == '$') {//imediate mode
                    temp.append("00");
                    std::string second = file_each_line.at(i).substr((int)index_space.at(1)+2, length - (int) index_space.at(1) - 2);
                    if (is_number(second)) { // if this is a number
                        if (atoi(second.c_str()) < 0) {
                            temp.append(two_complement(atoi(second.c_str())));
                        } else {
                            temp.append(converter_10_2(atoi(second.c_str())));
                        }
                    } else {
                        temp.append(converter_10_2(atoi(general[second].c_str())));
                    }
                } else if (file_each_line.at(i).at((int)index_space.at(1)+1) == 'R') {//Register Mode
                    temp.append("01");
                    std::string second = file_each_line.at(i).substr((int)index_space.at(1)+1, length - (int) index_space.at(1) - 1);
                    temp.append(general[second]);
                } else if (file_each_line.at(i).at(length-1) == ')') {//Indexed Mode
                    temp.append("11");
                    std::string second_reg = file_each_line.at(i).substr(length-3, 2);     
                    std::string second_num = file_each_line.at(i).substr((int)index_space.at(1)+1, length - 4 - ((int)index_space.at(1)) - 1 );  
                    if (atoi(second_num.c_str()) < 0) {
                        temp.append(two_complement(atoi(second_num.c_str())));
                    } else {
                        temp.append(converter_10_2(atoi(second_num.c_str())));
                    }
                    temp.append(general[second_reg]);            
                } else { // Direct Mode
                    temp.append("10");
                    std::string second = file_each_line.at(i).substr((int)index_space.at(1)+1, length - (int) index_space.at(1) - 1);
                    temp.append(converter_10_2(atoi(general[second].c_str())));
                }
                //Add zeros make it a multiple of eight and push back
                //std::cout << "Before adding zeros, temp is " << temp << std::endl;
                int final_length = (int) temp.length();
                //std::cout << "Before adding zeros, the length of temp is " << final_length << std::endl;
                int left_over = final_length % 8;
                //std::cout << "The left over is " << left_over << std::endl;
                while (left_over != 8 && left_over != 0) {
                    temp.append("0");
                    left_over ++;
                }
                final_length = (int) temp.length(); //Not a typo, but to update
                //std::cout << "After adding zeros, the length is " << final_length << std::endl;
                //int num_of_bytes = (final_length / 8);
                //byte += num_of_bytes;
                // Need to push back
                int i = 0;
                while (i < final_length) {
                    //std::cout << "Now we append " << temp.substr(i,8) << " while i is " << i << std::endl;
                    final_result.push_back(temp.substr(i,8));   
                    i += 8;                
                }
            } 



            if ((int) index_space.size() == 1) {// One operand operation
                int length_of_operand_code = index_space.at(0) - 4;
                std::string cur_instruction;
                if (length_of_operand_code == 4) {
                    cur_instruction = file_each_line.at(i).substr(4,4);
                } else if (length_of_operand_code == 3) {
                    cur_instruction = file_each_line.at(i).substr(4,3);
                } else if (length_of_operand_code == 2) {
                    cur_instruction = file_each_line.at(i).substr(4,2);
                } else {
                    cur_instruction = file_each_line.at(i).substr(4,1);
                }
                temp.append(general[cur_instruction]);
                int length = (int) file_each_line.at(i).length();
                if (file_each_line.at(i).at((int)index_space.at(0)+1) == '$') {//imediate mode
                    temp.append("00");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+2, length - (int) index_space.at(0) - 2);
                    if (is_number(first)) { // if this is a number
                        if (atoi(first.c_str()) < 0) {
                            temp.append(two_complement(atoi(first.c_str())));
                        } else {
                            temp.append(converter_10_2(atoi(first.c_str())));
                        }
                    } else {
                        temp.append(converter_10_2(atoi(general[first].c_str())));
                    }
                } else if (file_each_line.at(i).at((int)index_space.at(0)+1) == 'R') {//Register Mode
                    temp.append("01");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, length - (int) index_space.at(0) - 1);
                    temp.append(general[first]);
                } else if (file_each_line.at(i).at(length-1) == ')') {//Indexed Mode
                    temp.append("11");
                    std::string first_reg = file_each_line.at(i).substr(length-3, 2);     
                    std::string first_num = file_each_line.at(i).substr((int)index_space.at(0)+1, length - 4 - ((int)index_space.at(0)) - 1 );  
                    if (atoi(first_num.c_str()) < 0) {
                        temp.append(two_complement(atoi(first_num.c_str())));
                    } else {
                        temp.append(converter_10_2(atoi(first_num.c_str())));
                    }
                    temp.append(general[first_reg]);            
                } else { // Direct Mode
                    temp.append("10");
                    std::string first = file_each_line.at(i).substr((int)index_space.at(0)+1, length - (int) index_space.at(0) - 1);
                    temp.append(converter_10_2(atoi(general[first].c_str())));
                }
                int final_length = (int) temp.length();

                int left_over = final_length % 8;
                //std::cout << "The left over is " << left_over << std::endl;
                while (left_over != 8 && left_over != 0) {
                    temp.append("0");
                    left_over ++;
                }
                final_length = (int) temp.length(); //Not a typo, but to update
                //int num_of_bytes = (final_length / 8);
                //byte += num_of_bytes;
                // Need to push back
                int i = 0;
                while (i < final_length) {
                    final_result.push_back(temp.substr(i,8));   
                    i += 8;                
                }
            }
            
                
            if ((int) index_space.size() == 0) {//Zero Operand Operation
                std::string cur_instruction = file_each_line.at(i).substr(4,3);
                temp.append(general[cur_instruction]);
                int final_length = (int) temp.length();
                int left_over = final_length % 8;
                //std::cout << "The left over is " << left_over << std::endl;
                while (left_over != 8 && left_over != 0) {
                    temp.append("0");
                    left_over ++;
                }
                final_length = (int) temp.length(); //Not a typo, but to update
                //int num_of_bytes = (final_length / 8);
                //byte += num_of_bytes;
                // Need to push back
                int i = 0;
                while (i < final_length) {
                    final_result.push_back(temp.substr(i,8));   
                    i += 8;                
                }

            }       
                

        }

    }
}


//print out the machine code
void do_print() {
    int length = (int) final_result.size();
    std::string final_byte = std::to_string(length);
    for (int i = 0; i < length; i ++) {
        std::string i_length = std::to_string(i);
        int num_of_space = (int) final_byte.length() + 1 - (int) i_length.length();
        std::cout << "Byte";
        while (num_of_space != 0) {
            std::cout << " ";
            num_of_space --;
        }
        std::cout << i << ": " << final_result.at(i) << std::endl;
    }  
}


void do_save() {
    int length = (int) final_result.size();
    std::ofstream output_file ("output_my_test.txt");
    std::string final_byte = std::to_string(length);
    for (int i = 0; i < length; i++) {
        std::string i_length = std::to_string(i);
        int num_of_space = (int) final_byte.length() + 1 - (int) i_length.length();
        output_file << "Byte";
        while (num_of_space != 0) {
            output_file << " ";
            num_of_space --;
        }
        output_file << i << ": " << final_result.at(i) << std::endl;
    }
}

int main (int argc, char** argv) {
    //main local variables go here
    int input_return = 0;
    //input handling
    if (argc != 2) {
        std::cerr << "Wrong number of arguments." << std::endl;
        return 1;
    }
    std::string file_name = argv[1];
    input_return = handle_input(file_name);
    if (input_return != 0) {
        return input_return;
    }

    handle_general();
    set_up_labels();
    for (int i = 0; i < (int) labels.size(); i ++) {
        int count = 0;
        std::string temp = labels.at(i);
        for (int j = 0; j < (int) labels.size(); j ++) {
            if (labels.at(j) == temp) {
                count ++;
            }
        }
        if (count > 1) {
            std::cerr << "At least one duplicate label found." << std::endl;
            return 3;
        }
    }
    encoding();
    do_print();
    do_save();
    return 0;
}