# rapidjson2struct
convert rapidjson to struct 


[Changes](https://github.com/mklong/rapidjson2struct/blob/master/CHANGES.md)

### Usage

---


###### json : grade info 

```
{
  "monitor": {
    "id": 110,
    "name": "711"
  },
  "students": [
    {
      "id": 111,
      "name": "langke",
      "male": true
    },
    {
      "id": 112,
      "name": "guozu"
    }
  ],
  "rocket": true
}
```

###### define grade struct 

```
class grade : public base
{
public:
    grade(){}
    ~grade(){
        for(int i = 0 ;i < students.size();i ++)
            delete students[i];
    }

    student             monitor;
    vector<student* >   students;
    bool                rocket;

    virtual void parser_add_members(){

        add_struct_member(&monitor,"monitor",true);
        add_array_struct_member(&students,"students",alloc<student>,true);
        add_bool_member(rocket,"rocket");
    }
private:
    grade(const grade &);
    grade &operator= (const grade &);    
};
```
###### define student struct


```
struct student :public base{

    student():id(0),male(true){ }

    int     id;
    string  name;
    bool    male;

    virtual void parser_add_members(){
        add_number_member(id,"id",true);
        add_string_member(name,"name",true);
        add_bool_member(male,"male");
    }

};
```

###### parser

```
rapidjson::Document doc;
doc.Parse(json);
    
grade  g;
g.parser_add_members();
ret = g.parse(doc);
 
```



