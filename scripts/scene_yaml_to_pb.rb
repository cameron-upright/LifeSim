#!/usr/bin/env ruby

require 'optparse'
require 'yaml'

opt_parser = OptionParser.new do |opts|

  opts.banner = "Usage: scene_yaml_to_pb.rb <in.yaml> <out.prototxt>"

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


puts yaml.inspect

out = out_file


scene_obj_type_map = {"plane" => "PLANE", "box" => "BOX", "creature" => "CREATURE", "light" => "LIGHT"}


yaml.each do |obj|

  out.puts "scene_object {"

  if obj["type"] == "creature"
    out.puts "  config: \"#{obj["config"]}\""

  else
    out.puts "  desc {"

    out.puts "    type: #{scene_obj_type_map[obj["type"]]}"
    out.puts "    name: \"#{obj["name"]}\""

    if obj["type"] == "plane"
      out.puts "    [LifeSim.ScenePlaneDesc.scene_object] {"
      out.puts "      plane {"
      obj["plane"].each { |x| out.puts "        x: #{x}" }
      out.puts "      }"
      out.puts "      size: #{obj["size"]}"
      out.puts "      origin {"
      obj["origin"].each { |x| out.puts "        x: #{x}" }
      out.puts "      }"
      out.puts "    }"

    elsif obj["type"] == "box"
      out.puts "    [LifeSim.SceneBoxDesc.scene_object] {"
      out.puts "      transform {"
      out.puts "        position {"
      obj["position"].each { |x| out.puts "          x: #{x}" }
      out.puts "        }"
      out.puts "        rotation {"
      obj["rotation"].each { |x| out.puts "          x: #{x}" }
      out.puts "        }"
      out.puts "      }"
      out.puts "      half_extents {"
      obj["halfExtents"].each { |x| out.puts "        x: #{x}" }
      out.puts "      }"
      out.puts "    }"

    elsif obj["type"] == "light"
      out.puts "    [LifeSim.SceneLightDesc.scene_object] {"
      out.puts "      position {"
      obj["position"].each { |x| out.puts "          x: #{x}" }
      out.puts "      }"
      out.puts "      ambient {"
      obj["ambient"].each { |x| out.puts "          x: #{x}" }
      out.puts "      }"
      out.puts "      diffuse {"
      obj["diffuse"].each { |x| out.puts "          x: #{x}" }
      out.puts "      }"
      out.puts "      specular {"
      obj["specular"].each { |x| out.puts "          x: #{x}" }
      out.puts "      }"
      out.puts "    }"

    end

    out.puts "  }"
  end

  out.puts "}"

end
  
  
  
  
  out_file.close
