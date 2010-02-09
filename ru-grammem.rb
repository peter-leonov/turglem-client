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
    if word =~ /^[а-яА-Я]+$/
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
    
    arr << gramm.to_s(2)
    
    Grammems::BINARY.each do |sym, code|
      arr << Grammems::NAMES[sym] if gramm & code != 0
    end
    
    arr
  end
  
  def demo word
    vars = forms word
    vars.each do |var|
      puts "#{var[:gramms].join(", ")} #{var[:form]}"
    end
  end
  
end


class String
  RU_TRANSFORM =
  {
    'а' => 'А', 'А' => 'а',
    'б' => 'Б', 'Б' => 'б',
    'в' => 'В', 'В' => 'в',
    'г' => 'Г', 'Г' => 'г',
    'д' => 'Д', 'Д' => 'д',
    'е' => 'Е', 'Е' => 'е',
    'ё' => 'Ё', 'Ё' => 'ё',
    'ж' => 'Ж', 'Ж' => 'ж',
    'з' => 'З', 'З' => 'з',
    'и' => 'И', 'И' => 'и',
    'й' => 'Й', 'Й' => 'й',
    'к' => 'К', 'К' => 'к',
    'л' => 'Л', 'Л' => 'л',
    'м' => 'М', 'М' => 'м',
    'н' => 'Н', 'Н' => 'н',
    'о' => 'О', 'О' => 'о',
    'п' => 'П', 'П' => 'п',
    'р' => 'Р', 'Р' => 'р',
    'с' => 'С', 'С' => 'с',
    'т' => 'Т', 'Т' => 'т',
    'у' => 'У', 'У' => 'у',
    'ф' => 'Ф', 'Ф' => 'ф',
    'х' => 'Х', 'Х' => 'х',
    'ц' => 'Ц', 'Ц' => 'ц',
    'ч' => 'Ч', 'Ч' => 'ч',
    'ш' => 'Ш', 'Ш' => 'ш',
    'щ' => 'Щ', 'Щ' => 'щ',
    'ь' => 'Ь', 'Ь' => 'ь',
    'ы' => 'Ы', 'Ы' => 'ы',
    'ъ' => 'Ъ', 'Ъ' => 'ъ',
    'э' => 'Э', 'Э' => 'э',
    'ю' => 'Ю', 'Ю' => 'ю',
    'я' => 'Я', 'Я' => 'я'
  }
  
  alias :downcase_real :downcase
  def downcase
    cased = downcase_real
    cased.gsub(/([АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯ])/) do |w|
      RU_TRANSFORM[w]
    end
  end
  
  alias :upcase_real :upcase
  def upcase
    cased = upcase_real
    cased.gsub(/([абвгдеёжзийклмнопрстуфхцчшщьыъэюя])/) do |w|
      RU_TRANSFORM[w]
    end
  end
end




g = Grammemer.new
g.demo(ARGV[0])