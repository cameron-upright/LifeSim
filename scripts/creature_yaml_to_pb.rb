#!/usr/bin/env ruby

require 'optparse'
require 'yaml'

opt_parser = OptionParser.new do |opts|

  opts.banner = "Usage: creature_yaml_to_pb.rb <in.yaml> <out.prototxt>"

  opts.on_tail("-h", "--help", "Show this message") do
    puts opts
    exit
  end

end

opt_parser.parse(ARGV)

if ARGV.size != 2
  puts opt_parser
  exit
end


yaml = YAML.load(File.open(ARGV[0]))

out_file = File.open(ARGV[1], "w")



out = out_file


rigid_body_type_map = {"box" => "BOX"}
constraint_type_map = {"hinge" => "HINGE", "universal" => "UNIVERSAL"}

#puts yaml.inspect


out.puts "name: \"#{yaml["creature_name"]}\""
out.puts "type: CREATURE"
out.puts "[LifeSim.SceneCreatureDesc.scene_object] {"
out.puts "  transform {"
out.puts "    position { x: 0 x: 0 x: 0 }"
out.puts "    rotation { x: 0 x: 0 x: 0 x: 1 }"
out.puts "  }"

yaml["rigid_bodies"].each do |rb|

  out.puts "  child_scene_object {"
  out.puts "    type: #{rigid_body_type_map[rb["type"]]}"
  out.puts "    name: \"#{rb["name"]}\""
  out.puts "    is_foot: true" if rb["isFoot"]
  out.puts "    [LifeSim.SceneBoxDesc.scene_object] {"
  out.puts "      transform {"
  out.puts "        position {"
  rb["position"].each { |x| out.puts "          x: #{x}" }
  out.puts "        }"
  out.puts "        rotation {"
  rb["rotation"].each { |x| out.puts "          x: #{x}" }
  out.puts "        }"
  out.puts "      }"
  out.puts "      half_extents {"
  rb["halfExtents"].each { |x| out.puts "        x: #{x}" }
  out.puts "      }"
  out.puts "    }"
  out.puts "  }"

end


yaml["constraints"].each do |c|

  out.puts "  constraint {"
  out.puts "    type: #{constraint_type_map[c["type"]]}"
  out.puts "    name: \"#{c["name"]}\""
  out.puts "    body: \"#{c["bodyA"]}\"" unless c["bodyA"].nil?
  out.puts "    body: \"#{c["bodyB"]}\"" unless c["bodyB"].nil?

  if c["type"] == "universal"
    out.puts "    [LifeSim.UniversalConstraintDesc.constraint] {"
    out.puts "      pivot {"
    c["pivot"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      axis_0 {"
    c["axis0"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      axis_1 {"
    c["axis1"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      limit_0 {"
    c["limit0"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      limit_1 {"
    c["limit1"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "    }"

  elsif c["type"] == "hinge"

    out.puts "    [LifeSim.HingeConstraintDesc.constraint] {"
    out.puts "      axis_in_a {"
    c["axisA"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      axis_in_b {"
    c["axisB"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      pivot_in_a {"
    c["pivotA"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      pivot_in_b {"
    c["pivotB"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "      limit {"
    c["limit"].each { |x| out.puts "        x: #{x}" }
    out.puts "      }"
    out.puts "    }"

  end

  out.puts "  }"

end
  out.puts "}"

out_file.close
