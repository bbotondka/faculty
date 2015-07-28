/*global -$ */
'use strict';
// generated on 2015-07-28 using generator-drupal-gulp 1.0.0
var gulp = require('gulp');
var $ = require('gulp-load-plugins')();
var browserSync = require('browser-sync');
var reload = browserSync.reload;

gulp.task('scripts', function() {
    return gulp.src([
        'src/scripts/js/**/*'
    ], {
        base: 'src/scripts'
    }).pipe(gulp.dest('build/scripts'));
});

// styles task, will run when any SCSS files change & BrowserSync
// will auto-update browsers
gulp.task('styles', function () {
    return gulp.src('src/scss/**/*.scss')
        .pipe($.sourcemaps.init())
        .pipe($.sass({
            outputStyle: 'nested', // libsass doesn't support expanded yet
            precision: 10,
            includePaths: ['.'],
            errLogToConsole: true
            //onError: function (err) {console.error.bind(console, 'Sass error:'+err);}
        }))
        .pipe($.postcss([
            require('autoprefixer-core')({browsers: ['last 2 version']})
        ]))
        .pipe($.sourcemaps.write())
        .pipe(gulp.dest('build/css'))
        .pipe($.filter('scss**/*.css'))
        .pipe(browserSync.reload({stream:true}));
});

gulp.task('jshint', function () {
    return gulp.src('build/scripts/**/*.js')
        .pipe(reload({stream: true, once: true}))
        .pipe($.jshint())
        .pipe($.jshint.reporter('jshint-stylish'))
        .pipe($.if(!browserSync.active, $.jshint.reporter('fail')));
});


gulp.task('images', function () {
    return gulp.src('src/images/*')
        .pipe($.cache($.imagemin({
            progressive: true,
            interlaced: true,
            svgoPlugins: [{removeViewBox: false}]
        })))
        .pipe(gulp.dest('build/images'));
});

gulp.task('fonts', function () {
    return gulp.src('src/fonts/**/*')
        .pipe($.filter('**/*.{eot,svg,ttf,woff}'))
        .pipe(gulp.dest('build/fonts'));
});

gulp.task('extras', function () {
    return gulp.src([
        'src/*.*',
        'src/scripts/vendor/**/*'
    ], {
        base: 'src/',
        dot: true
    }).pipe(gulp.dest('build'));
});

gulp.task('bs-reload', function (){
    browserSync.reload();
});

gulp.task('browser-sync', function(){
    //watch files
    var files = [
        'build/css/**/*.css',
        'build/scripts/**/*js',
        'build/images/**/*',
        'templates/*.tpl.php'
    ];

    return browserSync.init(files, {
        proxy: "http://faculty.dev:8888", //change this to whatever your local development URL is.
        open: false,
        injectChanges: true
    });
});

gulp.task('watch',  ['images', 'fonts', 'styles', 'scripts', 'extras', 'browser-sync'],function () {

    gulp.watch('src/scripts/ts/**/*.ts', ['scripts']);
    gulp.watch('src/scss/**/*.scss', ['styles']);
    gulp.watch('images/**/*', ['images']);
    gulp.watch('src/scripts/vendor/**/*', ['extras']);
    gulp.watch('src/fonts/*', ['fonts']);
});

gulp.task('clearimages', function (done) {
    return $.cache.clearAll(done);
});

gulp.task('cleanFonts', require('del').bind(null, ['build/fonts']));

gulp.task('clean', require('del').bind(null, ['.tmp', 'build']));

gulp.task('build', ['images', 'fonts', 'styles', 'scripts', 'extras'], function () {
    return gulp.src('build/**/*').pipe($.size({title: 'build', gzip: true}));
});

gulp.task('default', ['clearimages', 'clean'], function () {
    gulp.start('build');
});
