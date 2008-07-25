#!/usr/bin/env ruby

data = File.read(ARGV[0])

magic_num, bsp_ver_num, number_groups = data.slice!(0,10).unpack('iiv')

puts "groups: #{number_groups}"

number_groups.times do |group_i|
  
  number_nodes = data.slice!(0,2).unpack('v')[0]

  puts "group: #{group_i}, nodes: #{number_nodes}"

  number_nodes.times do |node_i|
 
    x,y,z,o,f,b,c = data.slice!(0,28).unpack('eeeeiii')

    puts "node: #{node_i}, "+
         "x: #{x}, y: #{y}, z: #{z}, "+
         "offset: #{o}, front: #{f}, back: #{b}, "+
         "colour: #{c}"

  end

end

