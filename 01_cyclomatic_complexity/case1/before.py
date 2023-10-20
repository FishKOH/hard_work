choosen_point = None
need_choose = True

STATE_AT_HOME  = 0
STATE_TO_POINT = 1
STATE_TO_HOME  = 2
game_state = STATE_AT_HOME

def makeMove(tick_info):
    global choosen_point, need_choose, game_state
    updateWorld(tick_info)
    my_p = tick_info['players']['i']['position']
    if my_p == choosen_point:
        need_choose = True
        if game_state == 0:
            game_state = 1
        elif game_state == 1:
            game_state = 2
    if at_home(tick_info):
        if game_state == 2:
            game_state = 0
    if need_choose:
        if game_state == 0:
            choosen_point = go_from_home(tick_info)
        elif game_state == 1:
            choosen_point = choose_point(tick_info)
        elif game_state == 2:
            choosen_point = go_to_home(tick_info)
        else:
            choosen_point = possible_moves(my_p, players_dir['i'], tick_info['players']['i']['lines'])[0]
        need_choose = False
    # TODO choosen point == current point
    dir = go_to(tick_info, choosen_point)
    if is_reverse_dir(dir, players_dir['i']):
        dirs = possible_dirs(tick_info, possible_moves(my_p, players_dir['i'], tick_info['players']['i']['lines']))
        r_dir = rigth(dir)
        l_dir = left(dir)
        new_dir = r_dir if r_dir in dirs else l_dir
        dir = new_dir
    return commands[getDirectionFromPoint(dir)]

