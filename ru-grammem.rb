#!/usr/bin/env ruby1.9 -W0
# encoding: utf-8

require "grammemer"
require "ru-string"
require "optparse"

class CommandLineWrapper
  def run
    @options = {}
    
    OptionParser.new do |opts|
      opts.banner = "usage: #{$0} [--text-names] word"
      
      opts.on("-t", "--text", "textual names of grammems") do |v|
        @options[:text] = v
      end
    end.parse!
    
    process
  end
  
  def process
    g = Grammemer.new
    
    ARGV.each do |word|
      g.print_forms(word)
      puts
    end
    
    # if ARGV.length == 1
    #   g.print_forms(ARGV[0])
    # else
    #   g.get_forms(ARGV[0], ARGV[1].split(/\s*,\s*/))
    # end
  end
end

CommandLineWrapper.new.run