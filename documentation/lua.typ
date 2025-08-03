#import "cheatsheet.typ"

#set text(font: "Helvetica")
#show: cheatsheet.conf.with(
  title: text(fill: rgb("#000080"), "Hello, world !"),
  bg_color: white,
  fg_color: black,
  emphasis_color: rgb("#000080"),
)

#image("luaa.gif")

= Introduction
These are the basics: Lua is a highly embedable programming language.

== Types
#table(
  columns: (auto, auto),
  inset: 10pt,
  align: horizon,
  `nil`, [Represents 'nothing'],
  `boolean`, [`True` or `False`],
  `number`, [64-bit float or integer],
  `string`, [to represent text],
  `function`, [are function-as-values],
  `table`, [to represent complex data]
)

= Lorem
#lorem(10)

== More lorem
#lorem(90)

= Examples
```lua
function greet(name)
print("Hello, "..name.." !")
end

local name = "Bob"
greet(name) -- Hello, Bob !
```

== Comments
```lua
-- single-line
--[[
  multi-line
]]
```

== Array
```lua
local array = {1, 2, 3}
print(array[1]) -- arrays are 1-indexed !!
```

== Instanciation
#set par(justify: true)

#lorem(30)
