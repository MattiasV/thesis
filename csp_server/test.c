// Online C compiler to run C program online
#include <stdio.h>
#include <stdint.h>
#include <json-c/json.h>

#define JSON_FLAG JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY

int main(int argc, char* argv[]) {

  enum datatypes {none=0,u8=1,i8=2,u16=3,i16=4,u32=5,i32=6,f32=7,c=8,s=9,unk32=10,b = 11};

  struct json_parameters_t{
    char * parameter_key;
    uint8_t parameter_type;
  } json_parameters[] = {
    {"id", u8},
    {"description", s},
    {"datatype", u8},
    {"offset", u32},
    {"value", unk32},
    {"updated", b}
  };

  // struct json_object *jobj1;
  // struct json_object *jobj2;
  // struct json_object *jobj3;
  // struct json_object *jobj;
  //
	// const char *question1 = "Mum, clouds hide alien spaceships don't they ? 1";
	// const char *answer1 = "Of course not! (\"sigh\") 1";
  // const char *question2 = "Mum, clouds hide alien spaceships don't they ? 2";
	// const char *answer2 = "Of course not! (\"sigh\") 2";
  //
  // jobj = json_object_new_object();
  // jobj1 = json_object_new_object();
  // jobj2 = json_object_new_object();
  // jobj3 = json_object_new_array();
  //
	// json_object_object_add(jobj1, "question", json_object_new_string(question1));
	// json_object_object_add(jobj1, "answer", json_object_new_string(answer1));
  // json_object_object_add(jobj2, "question", json_object_new_string(question2));
  // json_object_object_add(jobj2, "answer", json_object_new_string(answer2));
  //
  // json_object_set_string(json_object_object_get(jobj1,"question"), "testtesttest");
  //
  // json_object_array_add(jobj3, jobj1);
  // json_object_array_add(jobj3, jobj2);
  //
  // json_object_object_add(jobj, "parameter", jobj3);
  // json_object_to_file_ext("testjson.json", jobj, JSON_FLAG);
  // printf("%s",json_object_to_json_string_ext(jobj, JSON_FLAG));
  //
  // json_object_put(jobj);

  return 0;
}
