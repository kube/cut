
      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

require 'pathname'

module CutRun
  class Directive
    PATTERN = /^\/\/\/CUTRUN:([A-Z_]+)\s+(.*)$/

    def initialize source_file, line
      @source_file = source_file
      @line = line

      parse
      execute
    end

    def parse
      @name, @argument = PATTERN.match(@line).captures
    end

    def execute
      case @name
        when "LINK_LIBRARY"
          @source_file.add_library @argument

        when "LIBRARY_FOLDER"
          path = @source_file.path.dirname.join(@argument)
          @source_file.add_library_path path

        when "INCLUDE_FOLDER"
          path = @source_file.path.dirname.join(@argument)
          @source_file.add_include_path path

        else
          puts "Unknown Cut Directive: #{@name}"
      end
    end

    def self.is line
      !!PATTERN.match(line)
    end
  end
end
