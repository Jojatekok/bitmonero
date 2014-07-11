#pragma once

#ifdef WIN32

#include "daemon/daemon.h"
#include "daemon/daemonizer.h"
#include "daemon/windows_service.h"
#include "daemon/windows_service_runner.h"

namespace daemonize
{
  namespace daemonizer
  {
    namespace
    {
      const command_line::arg_descriptor<bool> arg_install_service = {
        "install-service"
      , "Install Windows service"
      };
      const command_line::arg_descriptor<bool> arg_uninstall_service = {
        "uninstall-service"
      , "Uninstall Windows service"
      };
      const command_line::arg_descriptor<bool> arg_start_service = {
        "start-service"
      , "Start Windows service"
      };
      const command_line::arg_descriptor<bool> arg_stop_service = {
        "stop-service"
      , "Stop Windows service"
      };
      const command_line::arg_descriptor<bool> arg_is_service = {
        "run-as-service"
      , "Hidden -- true if running as windows service"
      };

      std::string get_argument_string(int argc, char const * argv[])
      {
        std::string result = "";
        for (int i = 1; i < argc; ++i)
        {
          result += " " + std::string{argv[i]};
        }
        return result;
      }
    }

    void init_options(
        boost::program_options::options_description & hidden_options
      , boost::program_options::options_description & normal_options
      , boost::program_options::options_description & configurable_options
      )
    {
      command_line::add_arg(normal_options, arg_install_service);
      command_line::add_arg(normal_options, arg_uninstall_service);
      command_line::add_arg(normal_options, arg_start_service);
      command_line::add_arg(normal_options, arg_stop_service);
      command_line::add_arg(hidden_options, arg_is_service);
    }

    template <typename T_executor>
    bool daemonize(
        int argc, char const * argv[]
      , T_executor & executor
      , boost::program_options::variables_map const & vm
      )
    {
      std::string arguments = get_argument_string(argc, argv);

      if (command_line::arg_present(vm, arg_install_service))
      {
        if (windows::ensure_admin(arguments))
        {
          arguments += " --run-as-service";
          return windows::install_service(T_executor::name, arguments);
        }
      }
      else if (command_line::arg_present(vm, arg_uninstall_service))
      {
        if (windows::ensure_admin(arguments))
        {
          return windows::uninstall_service(T_executor::name);
        }
      }
      else if (command_line::arg_present(vm, arg_start_service))
      {
        if (windows::ensure_admin(arguments))
        {
          return windows::start_service(T_executor::name);
        }
      }
      else if (command_line::arg_present(vm, arg_stop_service))
      {
        if (windows::ensure_admin(arguments))
        {
          return windows::stop_service(T_executor::name);
        }
      }
      else if (command_line::arg_present(vm, arg_is_service))
      {
        //LOG_PRINT_L0(CRYPTONOTE_NAME << " v" << PROJECT_VERSION_LONG);
        windows::t_service_runner<T_executor>::run(
          name
        , executor.create_daemon(vm)
        );
        return true;
      }
      else // interactive
      {
        //LOG_PRINT_L0(CRYPTONOTE_NAME << " v" << PROJECT_VERSION_LONG);
        return executor.run_interactive(vm);
      }
    }
  }
}

#endif