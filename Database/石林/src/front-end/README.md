# Front End

### Dependency
Materialize + Vue.js 2

### Install
```bash
npm install
# The reason of this shit is below.
echo "Object.defineProperty(window,'Vel',{get(){return window.Velocity}});" >> node_modules/materialize-css/js/velocity.min.js
echo "var Vel = window.Vel;" >> node_modules/materialize-css/bin/materialize.min.js
```

### Notes
As `Materialize` dose not work very well with `Webpack`, we need to perform two dirty patch on `Materialize`'s code.

1. Add `Object.defineProperty(window,'Vel',{get(){return window.Velocity}});` to the end of `./node_modules/materialize-css/js/velocity.min.js`
2. Add `var Vel = window.Vel;` to the end of `./node_modules/materialize-css/bin/materialize.min.js`
