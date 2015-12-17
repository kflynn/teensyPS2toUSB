keysPressed = [ 0 for x in range(6) ]

def MAKE(usbCode, need):
  for i in range(6):
    if keysPressed[i] == 0:
      keysPressed[i] = usbCode
      break
    elif keysPressed[i] == usbCode:
      break

  print("MAKE  %02X: %s" % (usbCode, ", ".join(["%02X" % x for x in keysPressed])))

  if keysPressed != need:
    print("!!!! need %s" % ", ".join(["%02X" % x for x in need]))

def BREAK(usbCode, need):
  i = 0
  j = 0

  try:
    while i < 6:
      # print("B %02X i %d KPi %02X j %d" % (usbCode, i, keysPressed[i], j))

      if keysPressed[i] == 0:
        break
      elif keysPressed[i] != usbCode:
        j += 1

      i += 1

      if i != j:
        keysPressed[j] = keysPressed[i]

    while j < 6:
      keysPressed[j] = 0
      j += 1
  except IndexError:
    print("IndexError! i %d j %d" % (i, j))

  print("BREAK %02X: %s" % (usbCode, ", ".join(["%02X" % x for x in keysPressed])))

  if keysPressed != need:
    print("!!!! need %s" % ", ".join(["%02X" % x for x in need]))

MAKE(1,  [ 1, 0, 0, 0, 0, 0 ])
BREAK(1, [ 0, 0, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 0, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 0, 0, 0, 0, 0 ])
BREAK(1, [ 0, 0, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 0, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
BREAK(1, [ 2, 0, 0, 0, 0, 0 ])
BREAK(2, [ 0, 0, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 0, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(1,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
MAKE(2,  [ 1, 2, 0, 0, 0, 0 ])
BREAK(2, [ 1, 0, 0, 0, 0, 0 ])
BREAK(1, [ 0, 0, 0, 0, 0, 0 ])
MAKE(6,  [ 6, 0, 0, 0, 0, 0 ])
MAKE(4,  [ 6, 4, 0, 0, 0, 0 ])
MAKE(1,  [ 6, 4, 1, 0, 0, 0 ])
MAKE(1,  [ 6, 4, 1, 0, 0, 0 ])
MAKE(4,  [ 6, 4, 1, 0, 0, 0 ])
MAKE(2,  [ 6, 4, 1, 2, 0, 0 ])
MAKE(3,  [ 6, 4, 1, 2, 3, 0 ])
BREAK(1, [ 6, 4, 2, 3, 0, 0 ])
BREAK(2, [ 6, 4, 3, 0, 0, 0 ])
BREAK(3, [ 6, 4, 0, 0, 0, 0 ])
BREAK(4, [ 6, 0, 0, 0, 0, 0 ])
BREAK(5, [ 6, 0, 0, 0, 0, 0 ])
BREAK(5, [ 6, 0, 0, 0, 0, 0 ])
BREAK(5, [ 6, 0, 0, 0, 0, 0 ])
BREAK(5, [ 6, 0, 0, 0, 0, 0 ])
BREAK(5, [ 6, 0, 0, 0, 0, 0 ])
BREAK(5, [ 6, 0, 0, 0, 0, 0 ])
BREAK(6, [ 0, 0, 0, 0, 0, 0 ])
