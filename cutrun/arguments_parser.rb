
      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

module ArgumentsParser

  def self.flag? arg
    arg.start_with? '-'
  end

  def self.get_flag_name arg
    arg.start_with?('--') ? arg : arg[0..1]
  end

  def self.get_flag_value arg
    if arg.start_with? '--' or arg.size == 2
      value = ARGV.shift
      abort "Expected value for flag #{arg}" if value.nil? or flag? value
    else
      value = arg[2..-1]
    end
    value
  end

  def self.get_complete_flag_name flag
    aliases = {
      '-I' => '--include-path',
      '-L' => '--library-path',
      '-l' => '--library'
    }
    aliases[flag] ? aliases[flag] : flag
  end

  def self.parse
    # Default options values
    options = {
      'files' => [],
      'libraries' => [],
      'include_path' => [],
      'library_path' => [],
      'out_folder' => './.cutrun_objects/'
    }

    # Parse all arguments
    loop do
      argument = ARGV.shift
      break if argument.nil?

      if flag? argument
        flag = get_flag_name argument

        case get_complete_flag_name flag
          when '--include-path'
            flag_value = get_flag_value argument
            options['include_path'] << flag_value

          when '--library-path'
            flag_value = get_flag_value argument
            options['library_path'] << flag_value

          when '--library'
            flag_value = get_flag_value argument
            options['libraries'] << flag_value

          when '--only-expand-macros'
            options['only_expand_macros'] = true

          else
            abort "Unknown flag #{flag}"
        end

      else
        options['files'] << argument
      end
    end

    options
  end
end
