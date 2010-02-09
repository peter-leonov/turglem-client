#!/usr/bin/env ruby1.9
# encoding: utf-8

class Grammemer
  module Config
    LEMMATIZER = "./turglem-client"
  end
  
  module Grammems
    BINARY =
    {
      :MC   => 0x001,
      :EC   => 0x002,
      
      :IP   => 0x004,
      :RP   => 0x008,
      :DP   => 0x010,
      :VP   => 0x020,
      :TP   => 0x040,
      :PP   => 0x080,
      :ZP   => 0x100,
      
      :MR   => 0x200,
      :JR   => 0x400,
      :SR   => 0x800
    }
    
    NAMES =
    {
      :MC   => "мн. ч.",
      :EC   => "ед. ч.",
      
      :IP   => 'им. п.',
      :RP   => 'род. п.',
      :DP   => 'дат. п.',
      :VP   => 'вин. п.',
      :TP   => 'тв. п.',
      :PP   => 'пр. п.',
      :ZP   => 'зв. п.',
      
      :MR   => "муж. р.",
      :JR   => "жен. р.",
      :SR   => "ср. р."
    }
  end
  
  def lemmatize word
    if word =~ /^[ёа-яЁА-Я]+$/
      `#{Config::LEMMATIZER} #{word}`
    else
      raise %Q{not a Russian word "#{word}"}
    end
  end
  
  def forms word
    raw = lemmatize word
    res = []
    raw.scan(/(\S+)\s+(\S+)\s+(\S+)(?:\s+(\*))?\n/) do |form, pos, gramm, original|
      res << {:form => form.downcase, :gramms => unpack_gramm(gramm.to_i(16))}
    end
    res
  end
  
  def unpack_gramm gramm
    arr = []
    
    # arr << gramm.to_s(2)
    
    Grammems::BINARY.each do |sym, code|
      arr << Grammems::NAMES[sym] if gramm & code != 0
    end
    
    arr
  end
  
  def print_forms word
    vars = forms word
    i = 0
    vars.each do |var|
      i += 1
      puts "#{var[:form].upcase}#{" " * (15 - var[:form].length)} #{var[:gramms].join(", ")}"
    end
  end
  
end