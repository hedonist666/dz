#!/usr/bin/env ruby

def print_int(v)
  "     printf(\"%d\\n\", #{v});\n"
end

def print_str(v)
  "     printStr(#{v});\n     putchar(\'\\n\');\n"
end

def print_str_p(v)
  "     printStr(*(#{v}));\n"
end

def scan_int(v)
  "     int #{v};\n     scanf(\"%d\",&#{v});\n"
end

def scan_s(v)
  "     char* #{v} = (char*) malloc(INPUT_BTS*sizeof(char));\n     scanf(\"%s\",#{v});\n"
end

def scan_c(v)
  "     char #{v} = getchar();\n"
end


def scan_str(v)
  "     Str #{v} = scanStr();\n"
end

def parse_declaration(str)
  str =~ /(.+) ([0-9a-zA-Z]+)\((.+)\)/
  args_type = $3.split ','
  ret_type = $1.strip
  fn_name = $2
  ints = 0
  strs = 0
  cs = 0
  chars = 0
  callee = "#{fn_name}("
  res = []
  unless str.include? "..."
    
    args_type.each do |t|
      
      case t
      when "int"
        ints += 1
        v = "v#{ints}"
        res << scan_int(v)
        callee << "#{v},"
      when "Str"
        strs += 1
        v = "str#{strs}"
        res << scan_str(v)
        callee << "#{v},"
      when "char*", "char *", "char * "
        cs += 1
        v = "s#{cs}"      
        res << scan_s(v)
        callee << "#{v},"
      when "char"
        chars += 1 
        v = "c#{chars}"
        res << scan_c(v)
        callee << "#{v},"
      else
        abort "NO IDEA 'BOUT ARG TYPE: #{t}"
      end

    end

  else
  
    res << scan_s("format")
    callee << "format,"
    puts "INPUT AR TYPE THEN ARGS 2 TIME (1 - Str, 2 - Str*, 3 - char*)"
    (1..2).each do |i|
      v = gets.chomp.to_i 
      case v
      when 1
        v = "str#{i}"
        res << scan_str(v)
        callee << "#{v},"
      when 2
        v = "str#{i}"
        res << scan_str(v)
        callee << "&#{v},"
      when 3
        v = "s#{i}"
        res << scan_s(v)
        callee << "#{v},"
      else 
        abort "UNKNOWN FORMAT"
      end
    end

  end  
  callee[-1] = ')'
#  callee << ";\n"
  case ret_type
  when "int"
    callee = print_int(callee)
  when "Str"
    callee = print_str(callee)
  when "Str*", "Str *", "Str * "
    callee = print_str_p(callee)
  else
    abort "UKNOWN RET TYPE: #{ret_type}"
  end
  res << callee
  return res.join
end

decls = []
outfn = "demo.c"
fn = "main.h"
prfn = ".prolog"
decl_regx = /(.+) ([0-9a-zA-Z]+)\((.+)\)/

abort "no prolog code" unless File.exists? prfn
abort "IDUNNOWHERSMAIN" unless File.exists? fn

output = File.open(outfn, 'w')
prolog = File.read(prfn)

output.write(prolog)

IO.readlines(fn).each do |line|

  if line =~ decl_regx
    decls << line
  end

end 

decls.filter! {|l| not l.include? "//ignore" }


i = 0

decls.each do |d|
  output.write "      \"#{i} - #{d.strip}\\n\"\n"
  i += 1
end 

output.write "  );\n  scanf(\"%d\", &f);\n  switch (f) {\n"

(0..i-1).each do |n|
  output.write "    case #{n}:\n    do {\n"
  output.write parse_declaration decls[n]
  output.write "    } while(0);\n    break;\n"
end

output.write "  }}\n"
 
output.write("}")
