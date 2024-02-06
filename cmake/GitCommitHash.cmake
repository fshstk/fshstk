####################################################################################################
#                ██████          █████                              █████    █████                 #
#               ███░░███        ░░███                              ░░███    ░░███                  #
#              ░███ ░░░   █████  ░███████      ██    ██     █████  ███████   ░███ █████            #
#             ███████    ███░░   ░███░░███    ░░    ░░     ███░░  ░░░███░    ░███░░███             #
#            ░░░███░    ░░█████  ░███ ░███                ░░█████   ░███     ░██████░              #
#              ░███      ░░░░███ ░███ ░███                 ░░░░███  ░███ ███ ░███░░███             #
#              █████     ██████  ████ █████    ██    ██    ██████   ░░█████  ████ █████            #
#            ░░░░░     ░░░░░░  ░░░░ ░░░░░    ░░    ░░    ░░░░░░     ░░░░░  ░░░░ ░░░░░              #
#                                                                                                  #
#           fantastic  spatial  holophonic               synthesis    tool    kit                  #
#                                                                                                  #
#                                   copyright (c) fabian hummel                                    #
#                                      www.github.com/fshstk                                       #
#                                          www.fshstk.com                                          #
#                                                                                                  #
#        this file is part of the fantastic spatial holophonic synthesis toolkit (fsh::stk)        #
# fsh::stk is free software: it is provided under the terms of the gnu general public license v3.0 #
#                                   www.gnu.org/licenses/gpl-3.0                                   #
####################################################################################################

# This function sets GIT_COMMIT_HASH to the short commit hash if the current commit is not tagged
# (i.e. a release), or to an empty string if it is tagged.

function(get_git_commit_hash GIT_COMMIT_HASH)
  # Check if the current commit has a tag
  execute_process(
    COMMAND git describe --exact-match --tags HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_TAG_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  # If the command succeeded, there is a tag, so set the commit hash to an empty string
  # Otherwise, get the commit hash and add parentheses around it
  if(GIT_TAG_RESULT EQUAL 0)
    set(${GIT_COMMIT_HASH} "" PARENT_SCOPE)
    message("Current commit is tagged, no GIT_COMMIT_HASH set")
  else()
    execute_process(
      COMMAND git rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH_LOCAL
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(${GIT_COMMIT_HASH} " (${GIT_COMMIT_HASH_LOCAL})" PARENT_SCOPE)
    message("GIT_COMMIT_HASH: (${GIT_COMMIT_HASH_LOCAL})")
  endif()
endfunction()
