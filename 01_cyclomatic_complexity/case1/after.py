choosen_point = None

STATE_UNKNOWN  = -1
STATE_AT_HOME  = 0
STATE_TO_POINT = 1
STATE_TO_HOME  = 2
game_state = STATE_AT_HOME



def random_move(tick_info):
    me_point = tick_info['players']['i']['position']
    return possible_moves(me_point, players_dir['i'], tick_info['players']['i']['lines'])[0]

def chooseTargetPoint(tick_info, prev_choosen_point, prev_game_state):
    def meAtPointPredicat():
        me_point = tick_info['players']['i']['position']
        return me_point == prev_choosen_point
    def meAtHomePredicat():
        return at_home(tick_info)
    
    targetsCompletePredicats = {STATE_AT_HOME : meAtPointPredicat, STATE_TO_POINT : meAtPointPredicat, STATE_TO_HOME : meAtHomePredicat}
    nextGameState = {STATE_AT_HOME : STATE_TO_POINT, STATE_TO_POINT : STATE_TO_HOME, STATE_TO_HOME : STATE_AT_HOME}
    pointChoosers = {STATE_AT_HOME : go_from_home, STATE_TO_POINT : choose_point, STATE_TO_HOME : go_to_home, STATE_UNKNOWN : random_move}
    
    if targetsCompletePredicats[prev_game_state]():
        game_state = nextGameState[prev_game_state]
        choosen_point = pointChoosers[game_state](tick_info)
        return game_state, choosen_point
    
    return prev_choosen_point, prev_game_state

def correctReverseDir(reverse_dir, tick_info):
    me_point = tick_info['players']['i']['position']
    dirs = possible_dirs(tick_info, possible_moves(me_point, players_dir['i'], tick_info['players']['i']['lines']))
    r_dir = rigth(reverse_dir)
    l_dir = left(reverse_dir)
    new_dir = r_dir if r_dir in dirs else l_dir
    return dir

def makeMove(tick_info):
    global choosen_point, game_state
    updateWorld(tick_info)
    choosen_point, game_state = chooseTargetPoint(tick_info, choosen_point, game_state)
    dir = go_to(tick_info, choosen_point)
    if is_reverse_dir(dir, players_dir['i']):
        dir = correctReverseDir(dir)
    return commands[getDirectionFromPoint(dir)]
