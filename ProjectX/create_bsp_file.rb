#!/usr/bin/env ruby

# data file
data = File.read(ARGV[0])

# output file
out = File.open(ARGV[1],'wb')

# magic number
out.write "PRJX"

# bsp version
out.write [1].pack('i')

# number of groups
data.slice!(/^groups: ([0-9]+)$/)
number_groups = $1.to_i
out.write [number_groups].pack('v')

# groups
number_groups.times do |group_i|
  
  # number of nodes
  data.slice!(/^group: ([0-9]+), nodes: ([0-9]+)$/)
  group_index  = $1.to_i
  number_nodes = $2.to_i
  out.write [number_nodes].pack('v')

  # nodes
  number_nodes.times do |node_i|
 
    # pull values from node
    # most are integers that can have a negative sign thus: [0-9-]+
    # rest are floats that can be represented like, "-1.37524310162007e-07" thus: ([0-9\.-e]+)
    data.slice!(/^node: ([0-9]+), x: ([0-9\.\-eE]+), y: ([0-9\.\-eE]+), z: ([0-9\.\-eE]+), offset: ([0-9\.\-eE]+), front: ([0-9]+), back: ([0-9]+), colour: ([0-9-]+)$/)
    node_index = $1.to_i
    x,y,z,o,f,b,c = $2.to_f, $3.to_f, $4.to_f, $5.to_f, $6.to_i, $7.to_i, $8.to_i
    out.write [x,y,z,o,f,b,c].pack('eeeeiii')

  end

end

