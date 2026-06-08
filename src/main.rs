mod cli;
mod common;
mod lexer;

use std::{env, fs};
use colored::Colorize;
use codespan_reporting::files::SimpleFiles;
use codespan_reporting::term::termcolor::ColorSpec;
use codespan_reporting::term::{self, Chars, Config, Styles};
use codespan_reporting::term::termcolor::{ColorChoice, StandardStream};

fn main() {
    let command = match cli::parse_command(env::args()) {
        Ok(o) => o,
        Err(err) => {
            eprintln!("{}: {err}", "error".red().bold().underline());
            return;
        }
    };
    colored::control::set_override(!command.no_color);

    let contents = match fs::read_to_string(&command.input) {
        Ok(o) => o,
        Err(err) => {
            eprintln!("{}: {}", "error".red().bold().underline(), err.to_string());
            return;
        }
    };
    
    let mut files = SimpleFiles::new();
    let file_id = files.add(&command.input, &contents);
    let config = {
        let mut styles = Styles::default();
        {
            let mut cspec = ColorSpec::new();
            cspec.set_fg(Some(term::termcolor::Color::Red))
                .set_bold(true)
                .set_underline(true);
            styles.header_error = cspec;
        }
        styles.header_message = ColorSpec::new();
        let chars = Chars {
            source_border_top_left: ',',
            source_border_top: '-',
            source_border_left: '|',
            source_border_left_break: ':',
            note_bullet: '*',
            single_primary_caret: '^',
            single_secondary_caret: '~',
            multi_primary_caret_start: '^',
            multi_primary_caret_end: '^',
            multi_secondary_caret_start: '\'',
            multi_secondary_caret_end: '\'',
            multi_top_left: ',',
            multi_top: '-',
            multi_bottom_left: '\'',
            multi_bottom: '-',
            multi_left: '|',
            pointer_left: '|',
        };
        Config {
            styles,
            chars,
            ..Config::default()
        }
    };

    let mut writer = StandardStream::stderr(
        if command.no_color { ColorChoice::Never } 
        else { ColorChoice::Always }
    );

    let mut rodeo = lasso::Rodeo::new();
    let tokens = match lexer::tokenize(file_id, &contents, &mut rodeo) {
        Ok(o) => o,
        Err(err) => {
            let _ = term::emit(&mut writer, &config, &files, &err);
            return;
        }
    };
    println!("tokens: {tokens:#?}");
}