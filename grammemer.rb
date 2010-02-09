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
  
  def get_variants word
    raw = lemmatize word
    res = []
    variant = nil
    raw.scan(/^(\d+)\.$|^(\S+)\s+(\S+)\s+(\S+)(?:\s+(\*))?$/) do |nxt, form, pos, gramm, original|
      if nxt
        variant = []
        res << variant
        next
      end
      variant << {:form => form.downcase, :gramms => unpack_gramm(gramm.to_i(16))}
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
    i = 0
    get_variants(word).each do |variant|
      i += 1
      puts "#{i}.\n"
      variant.each do |form|
        puts "#{form[:form].upcase}#{" " * (15 - form[:form].length)} #{form[:gramms].join(", ")}"
      end
    end
  end
  
end