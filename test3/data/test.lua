--lua 打印lua script
print("lua script") 
--调用成员变量m_test_string(test_str为注册的名字)
print(test_lua.test_str)
--调用c++静态变量（需要加上test命名空间）
test.test_lua.static_data=12
print("static_data: "..test.test_lua.static_data)
--调用c++类test_lua属性name
test_lua.name="name_property";
print("name: "..test_lua.name);
--lua调用c++方法test_lua为c++类在lua的注册名，调用test方法
test_lua:test(3,4)

--调用c++调用方法返回多个值
local ret1,ret2 = test_lua:cFunc()
print("ret1="..ret1.." ret2="..ret2)

--创建test_lua_child对象
local test_lua_child = test.test_lua_child("test_string")
--调用其变量
print("child string:"..test_lua_child.test_child_string);
--调用父类的name属性
test_lua_child.name="child_name_property";
print("name:"..test_lua_child.name);

--lua 方法加法
function lua_add_function(a,b)
    print("lua_add_function") 
    return a+b;
end

--lua 方法字符串加法（..是相加语法）
function lua_add_str_function(a,b)
    print("lua_add_str_function") 
    return a..b;
end

lua_add_function(4,6);