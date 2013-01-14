// ******************************************************************************** //
// test.h																			//
// ======																			//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: List of all test scenarios, which can be called from the test			//
//			application.															//
//																					//
// ******************************************************************************** //

// Test of OrE::ADT::List
void test_list();

// Test of OrE::ADT::AVLTree and OrE::ADT::LinkedAVLTree
void test_avl();

// Benchmark of OrE::ADT::Hashmap
void test_hashmap();

// Test and benchmark of Math:: functions
void test_math();

// Test of OrE::ADT::MultiMap
void test_multimap();

// Test of OrE::String::Tokenizer
void test_tokenizer();

// Test and Benchmark of the fibonacci heap
void test_heap();

// Test of OrE::ADT::Graph
void test_graph();