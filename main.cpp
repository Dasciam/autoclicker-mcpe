#include <iostream>
#include <csignal>
#include <thread>
#include <random>

#include "args.hxx"

#include "xutils.hpp"
#undef None // X things...

static const std::string APP_NAME = "Minecraft";

static constexpr int MIN_CPS_DEFAULT = 14;
static constexpr int MAX_CPS_DEFAULT = 16;

std::chrono::nanoseconds FrequencyToNanoseconds(int64_t freq) {
  return std::chrono::nanoseconds{1000000000 / freq};
}

struct {
  std::mutex SleepMu;

  int MinCPS{};
  int MaxCPS{};

  std::atomic_bool Started = true;

  Display *Display_ = nullptr;
} Ctx;

void SignalHandler(int) {
  Ctx.Started.store(false);

  Ctx.SleepMu.lock();

  xutils::CloseDisplaySession(Ctx.Display_);

  Ctx.SleepMu.unlock();

  std::cout << std::endl << "Program will exit" << std::endl;
}

int main(int argc, char *argv[]) {
  args::ArgumentParser parser("Minecraft: Bedrock Edition auto-clicker.");

  args::HelpFlag help(parser, "help", "Display help menu", {'h', "help"});

  args::ValueFlag<int> minCps(
	  parser,
	  "min-cps",
	  "Minimum of clicks per second",
	  {"min-cps"},
	  MIN_CPS_DEFAULT,
	  args::Options::None
  );

  args::ValueFlag<int> maxCps(
	  parser,
	  "max-cps",
	  "Maximum of clicks per second",
	  {"max-cps"},
	  MAX_CPS_DEFAULT,
	  args::Options::None
  );

  try {
	parser.ParseCLI(argc, argv);
  } catch (args::Help &) {
	std::cout << parser;
	return EXIT_SUCCESS;
  } catch (args::ParseError &err) {
	std::cerr << err.what() << std::endl;
	std::cerr << parser;
	return EXIT_SUCCESS;
  }

  Ctx.MinCPS = args::get(minCps);
  Ctx.MaxCPS = args::get(maxCps);

  std::cout << "Min CPS: " << Ctx.MinCPS << std::endl;
  std::cout << "Max CPS: " << Ctx.MaxCPS << std::endl;

  Ctx.Display_ = xutils::GetDefaultDisplay();

  std::signal(SIGINT, SignalHandler);
  std::signal(SIGKILL, SignalHandler);
  std::signal(SIGTERM, SignalHandler);
  std::signal(SIGABRT, SignalHandler);
  std::signal(SIGSTOP, SignalHandler);

  std::thread(
	  []() -> void {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int64_t> dis(Ctx.MinCPS, Ctx.MaxCPS);

		while (Ctx.Started.load()) {
		  Ctx.SleepMu.lock();

		  auto window = xutils::GetActiveWindow(Ctx.Display_);
		  if (xutils::IsLeftMouseButtonPressed(Ctx.Display_)
			  && xutils::GetWindowTitle(Ctx.Display_, window) == APP_NAME) {
			xutils::SimulateLeftMouseButtonClick(Ctx.Display_, window);
		  }

		  Ctx.SleepMu.unlock();

		  std::this_thread::sleep_for(FrequencyToNanoseconds(dis(gen)));
		}
	  }
  ).detach();

  while (Ctx.Started.load()) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}
