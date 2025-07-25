#let black = rgb("#000000")
#let dark-blue = rgb("#1D2B53")
#let dark-purple = rgb("#7E2553")
#let dark-green = rgb("#008751")
#let brown = rgb("#AB5236")
#let dark-grey = rgb("#5F574F")
#let light-grey = rgb("#C2C3C7")
#let white = rgb("#FFF1E8")
#let red = rgb("#FF004D")
#let orange = rgb("#FFA300")
#let yellow = rgb("#FFEC27")
#let green = rgb("#00E436")
#let blue = rgb("#29ADFF")
#let lavender = rgb("#83769C")
#let pink = rgb("#FF77A8")
#let light-peach = rgb("#FFCCAA")

#let comment(content) = {
  text(
    fill: lavender,
    "-- " + content
  )
}

#let conf(
  title: "",
  doc) = {

  set page(
    paper: "a3",
    flipped: true,
    columns: 6,
    margin: 1.5cm,
    background: rect(width: 100%, height: 100%, stroke: 5pt + red, outset: -0.5cm),
    fill: light-peach.lighten(70%)
  )
  
  set columns(gutter: 0.8cm)
  set text(
    font:"Adwaita Mono"
  )

  show heading: it => {
    upper(text(fill: red, it))
  } 

  place(
    top + left,
    float: true,
    scope: "parent",
    clearance: 2em
  )[
    #text(
      fill: dark-blue,
      weight: 700,
      size: 24pt,
      title
    )
  ]

  doc
}
