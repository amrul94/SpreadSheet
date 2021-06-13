// Generated from Formula.g4 by ANTLR 4.9
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class FormulaLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.9", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, NUMBER=3, ADD=4, SUB=5, MUL=6, DIV=7, CELL=8, WS=9;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"T__0", "T__1", "INT", "UINT", "EXPONENT", "NUMBER", "ADD", "SUB", "MUL", 
			"DIV", "CELL", "WS"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'('", "')'", null, "'+'", "'-'", "'*'", "'/'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, "NUMBER", "ADD", "SUB", "MUL", "DIV", "CELL", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public FormulaLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "Formula.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\u608b\ua72a\u8133\ub9ed\u417c\u3be7\u7786\u5964\2\13S\b\1\4\2\t\2\4"+
		"\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13\t"+
		"\13\4\f\t\f\4\r\t\r\3\2\3\2\3\3\3\3\3\4\5\4!\n\4\3\4\3\4\3\5\6\5&\n\5"+
		"\r\5\16\5\'\3\6\3\6\3\6\3\7\3\7\5\7/\n\7\3\7\5\7\62\n\7\3\7\3\7\3\7\5"+
		"\7\67\n\7\5\79\n\7\3\b\3\b\3\t\3\t\3\n\3\n\3\13\3\13\3\f\6\fD\n\f\r\f"+
		"\16\fE\3\f\6\fI\n\f\r\f\16\fJ\3\r\6\rN\n\r\r\r\16\rO\3\r\3\r\2\2\16\3"+
		"\3\5\4\7\2\t\2\13\2\r\5\17\6\21\7\23\b\25\t\27\n\31\13\3\2\7\4\2--//\3"+
		"\2\62;\4\2GGgg\3\2C\\\5\2\13\f\17\17\"\"\2X\2\3\3\2\2\2\2\5\3\2\2\2\2"+
		"\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2\25\3\2\2\2\2\27\3"+
		"\2\2\2\2\31\3\2\2\2\3\33\3\2\2\2\5\35\3\2\2\2\7 \3\2\2\2\t%\3\2\2\2\13"+
		")\3\2\2\2\r8\3\2\2\2\17:\3\2\2\2\21<\3\2\2\2\23>\3\2\2\2\25@\3\2\2\2\27"+
		"C\3\2\2\2\31M\3\2\2\2\33\34\7*\2\2\34\4\3\2\2\2\35\36\7+\2\2\36\6\3\2"+
		"\2\2\37!\t\2\2\2 \37\3\2\2\2 !\3\2\2\2!\"\3\2\2\2\"#\5\t\5\2#\b\3\2\2"+
		"\2$&\t\3\2\2%$\3\2\2\2&\'\3\2\2\2\'%\3\2\2\2\'(\3\2\2\2(\n\3\2\2\2)*\t"+
		"\4\2\2*+\5\7\4\2+\f\3\2\2\2,.\5\t\5\2-/\5\13\6\2.-\3\2\2\2./\3\2\2\2/"+
		"9\3\2\2\2\60\62\5\t\5\2\61\60\3\2\2\2\61\62\3\2\2\2\62\63\3\2\2\2\63\64"+
		"\7\60\2\2\64\66\5\t\5\2\65\67\5\13\6\2\66\65\3\2\2\2\66\67\3\2\2\2\67"+
		"9\3\2\2\28,\3\2\2\28\61\3\2\2\29\16\3\2\2\2:;\7-\2\2;\20\3\2\2\2<=\7/"+
		"\2\2=\22\3\2\2\2>?\7,\2\2?\24\3\2\2\2@A\7\61\2\2A\26\3\2\2\2BD\t\5\2\2"+
		"CB\3\2\2\2DE\3\2\2\2EC\3\2\2\2EF\3\2\2\2FH\3\2\2\2GI\t\3\2\2HG\3\2\2\2"+
		"IJ\3\2\2\2JH\3\2\2\2JK\3\2\2\2K\30\3\2\2\2LN\t\6\2\2ML\3\2\2\2NO\3\2\2"+
		"\2OM\3\2\2\2OP\3\2\2\2PQ\3\2\2\2QR\b\r\2\2R\32\3\2\2\2\f\2 \'.\61\668"+
		"EJO\3\b\2\2";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}