const path = require('path');

module.exports = {
    entry: "./src/entry.js",
    output: {
        path: `${__dirname}/static`,
        filename: "bundle.js"
    },
    module: {
        loaders: [
            { test: /\.css$/, loader: "style!css" },
            { test: /\.vue$/, loader: "vue" },
            { test: /\.js$/, loader: 'babel', exclude: /node_modules/ },
            { test: /\.(eot|woff|ttf|woff2)$/, loader: "file-loader" },
        ]
    },
    babel: {
        presets: ['es2015'],
        plugins: ['transform-runtime']
    },
    resolve: {
        extensions: ['', '.js', '.vue'],
        alias: {
            vue: 'vue/dist/vue.js',
            filter: path.join(__dirname, './src/filters'),
            components: path.join(__dirname, './src/components'),
        }
    },
    devServer: {
        historyApiFallback: true,
        hot: false,
        inline: true,
        grogress: true,
    },
};
